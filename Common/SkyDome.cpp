//////////////////////////////////////////////////////////////////////////////////////////
//						  Dynamic Terrain Generation Prototype							//
//					  Written 2007 by Jon Wills (jc@chromaticaura.net)						//
//				Written for a Win32 environment using the DirectSound API.				//
//																						//
//				   Written at the University of Abertay Dundee, Scotland				//
//////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////
//	SKY DOME CLASS																		//
//	Handles the prototype's skydome.													//
//////////////////////////////////////////////////////////////////////////////////////////
//	The majority of this code is taken from the Skydome demo by Luis R. Sempé of Sphere
//	games.  All code within functions is preserved as written by Sempé, all other
//	modifications are commented.  
//
//	http://www.spheregames.com/index.php?p=templates/pages/tutorials
//////////////////////////////////////////////////////////////////////////////////////////

// SkyDome Demo  -  October 2001
//
// Luis R. Sempé
// visual@spheregames.com
// Sphere Games (http://www.spheregames.com)
// 
// You may use, copy, distribute or create derivative software
// for any purpose. If you do, I'd appreciate it if you write me 
// and let me know what you do with it. Thanks!
// Have fun!
////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////
//	CLASS HEADER
//////////////////////////////////////////////////////////////////////////////////////////
#include "SkyDome.h"

//	Constructor.  
//////////////////////////////////////////////////////////////////////////////////////////
SkyDome::SkyDome(float radius, float dtheta, float dphi, float hTile, float vTile)
{
	// Creates the pointers for the two multitexturing functions.  
	glActiveTextureARB		= (PFNGLACTIVETEXTUREARBPROC)	wglGetProcAddress("glActiveTextureARB");
	glMultiTexCoord2fARB	= (PFNGLMULTITEXCOORD2FARBPROC)	wglGetProcAddress("glMultiTexCoord2fARB");

	// Creates the skydome.  
	this->GenerateDome(radius, dtheta, dphi, hTile, vTile);	// Generates the vertices for the skydome.  
	this->LoadTexture();			// Loads the sky texture.  
	this->GenerateDisplayList();	// Generates the Display List for the Skydome.  
}

//	Destructor.  
//////////////////////////////////////////////////////////////////////////////////////////
SkyDome::~SkyDome()
{
	// If the vertex list exists, deletes all the vertices.  
	if (this->Vertices)
	{
		delete this->Vertices;
		this->Vertices = NULL;
	}
}

void SkyDome::Render()
{
	glDisable(GL_LIGHTING);
	glPushMatrix();
	glTranslatef(0.0f, -100.0f, 0.0f);

	glBindTexture(GL_TEXTURE_2D, sky);
	glCallList(this->SkyDL);
		
	glPopMatrix();
	glEnable(GL_LIGHTING);
}

//	Function to generate the vertices.  Sempé's code is preserved where relevant.  
//////////////////////////////////////////////////////////////////////////////////////////
void SkyDome::GenerateDome(float radius, float dtheta, float dphi, float hTile, float vTile)
{
	int theta, phi;

	// Initialize our Vertex array
	NumVertices = (int)((360 / dtheta) * (90 / dphi) * 4);
	
	// JW - Creates & zero-fills a dynamic vertex list.  
	Vertices = new GLNode[NumVertices];
	ZeroMemory(Vertices, sizeof(GLNode) * NumVertices);

	// JW - Used to calculate the UV coordinates
	GLVertex V;

	// Generate the dome
	int n = 0;
	for (phi = 0; phi <= 90 - dphi ; phi += (int)dphi)
	{
		for (theta = 0 ; theta <= 360 - dtheta ; theta += (int)dtheta)
		{
			// Calculate the vertex at phi, theta
			Vertices[n].Position.x = sinf(phi * DTOR) * cosf(DTOR * theta);
			Vertices[n].Position.y = sinf(phi * DTOR) * sinf(DTOR * theta);
			Vertices[n].Position.z = cosf(phi * DTOR);
			Vertices[n].Position *= radius;

			// Create a vector from the origin to this vertex
			V = Vertices[n].Position;
			V.Normalise();		// Normalize the vector

			// Calculate the spherical texture coordinates
			Vertices[n].Texture.x = hTile * (float)(atan2(V.x, V.z)/(PI * 2)) + 0.5f;
			Vertices[n].Texture.y = vTile * (float)(asinf(V.y) / PI) + 0.5f;		
			n++;

			// Calculate the vertex at phi+dphi, theta
			Vertices[n].Position.x = sinf((phi + dphi) * DTOR) * cosf(theta * DTOR);
			Vertices[n].Position.y = sinf((phi + dphi) * DTOR) * sinf(theta * DTOR);
			Vertices[n].Position.z = cosf((phi + dphi) * DTOR);
			Vertices[n].Position *= radius;
			
			// Calculate the texture coordinates
			V = Vertices[n].Position;
			V.Normalise();

			Vertices[n].Texture.x = hTile * (float)(atan2(V.x, V.z)/(PI * 2)) + 0.5f;
			Vertices[n].Texture.y = vTile * (float)(asinf(V.y) / PI) + 0.5f;		
			n++;

			// Calculate the vertex at phi, theta+dtheta
			Vertices[n].Position.x = sinf(DTOR*phi) * cosf(DTOR*(theta+dtheta));
			Vertices[n].Position.y = sinf(DTOR*phi) * sinf(DTOR*(theta+dtheta));
			Vertices[n].Position.z = cosf(DTOR*phi);
			Vertices[n].Position *= radius;
			
			// Calculate the texture coordinates
			V = Vertices[n].Position;
			V.Normalise();

			Vertices[n].Texture.x = hTile * (float)(atan2(V.x, V.z)/(PI * 2)) + 0.5f;
			Vertices[n].Texture.y = vTile * (float)(asinf(V.y) / PI) + 0.5f;		
			n++;

			if (phi > -90 && phi < 90)
			{
				// Calculate the vertex at phi+dphi, theta+dtheta
				Vertices[n].Position.x = sinf((phi + dphi) * DTOR) * cosf(DTOR * (theta + dtheta));
				Vertices[n].Position.y = sinf((phi + dphi) * DTOR) * sinf(DTOR * (theta + dtheta));
				Vertices[n].Position.z = cosf((phi + dphi) * DTOR);
				Vertices[n].Position *= radius;
				
				// Calculate the texture coordinates
				V = Vertices[n].Position;
				V.Normalise();

				Vertices[n].Texture.x = hTile * (float)(atan2(V.x, V.z)/(PI*2)) + 0.5f;
				Vertices[n].Texture.y = vTile * (float)(asinf(V.y) / PI) + 0.5f;		
				n++;
			}
		}
	}

	// Fix the problem at the seam
	for (int i = 0; i < NumVertices - 3; i++)
	{
		this->CheckSeam(i,		i + 1	);
		this->CheckSeam(i + 1,	i		);
		this->CheckSeam(i,		i + 2	);
		this->CheckSeam(i + 2,	i		);
		this->CheckSeam(i + 1,	i + 2	);
		this->CheckSeam(i + 2,	i + 1	);
	}
}

//	Function to generate the Skydome's display list.  
//////////////////////////////////////////////////////////////////////////////////////////
void SkyDome::GenerateDisplayList()
{
	// If the display list already exists, delete it.
	if (!this->SkyDL)
		glDeleteLists(this->SkyDL, 1);

	// Generate the new list.  
	glNewList(this->SkyDL, GL_COMPILE);
		glRotatef(270, 1.0f, 0.0f, 0.0f);			// Rotate the dome 270 degrees to the right position.  

		//	Render all the vertices.  
		glBegin(GL_TRIANGLE_STRIP);
		for (int i = 0; i < NumVertices; i++)
		{
			glColor3f(1.0f, 1.0f, 1.0f);

			glTexCoord2f(Vertices[i].Texture.x, Vertices[i].Texture.y);
			Vertices[i].Draw();
		}
		glEnd();

	glEndList();
}

//	Function to load the sky texture.  DevIL is used in this function.  
//////////////////////////////////////////////////////////////////////////////////////////
void SkyDome::LoadTexture()
{
	ILuint texid;		// DevIL Texture ID.  

	ilInit();			// Initialises DevIL.
	
	// Generates & binds a DevIL image name.  
	ilGenImages(1, &texid);
	ilBindImage(texid);

	// Loads the image.  
	if ((ilLoadImage("textures/sky.jpg")) && ilConvertImage(IL_RGB, IL_UNSIGNED_BYTE))
	{	
		// If the image was loaded correctly...
		glActiveTextureARB(GL_TEXTURE0_ARB);			// Creates Texture Unit 0.
		glEnable(GL_TEXTURE_2D);						// Enable texturing.  
		glGenTextures(1, &this->sky);					// Create The Texture
		glBindTexture(GL_TEXTURE_2D, this->sky);		// Binds the texture.  
	    glTexImage2D(	GL_TEXTURE_2D, 0, ilGetInteger(IL_IMAGE_BPP), ilGetInteger(IL_IMAGE_WIDTH),
						ilGetInteger(IL_IMAGE_HEIGHT), 0, ilGetInteger(IL_IMAGE_FORMAT), GL_UNSIGNED_BYTE,
						ilGetData());
		
		// Sets linear filtering.  
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);	
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}

	// Deletes the DevIL image now that a copy has been uploaded to memory.  
	ilDeleteImages(1, &texid);
}

//	Function to check the seams around the edges of the skydome's vertices.  
//////////////////////////////////////////////////////////////////////////////////////////
void SkyDome::CheckSeam(int a, int b)
{
	if (Vertices[a].Texture.x - Vertices[b].Texture.x > 0.9f)
		Vertices[b].Texture.x += 1.0f;
	if (Vertices[a].Texture.y - Vertices[b].Texture.y > 0.8f)
		Vertices[b].Texture.y += 1.0f;
}