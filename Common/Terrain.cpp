//////////////////////////////////////////////////////////////////////////////////////////
//						  Dynamic Terrain Generation Prototype							//
//					  Written 2007 by Jon Wills (jc@chromaticaura.net)						//
//				Written for a Win32 environment using the DirectSound API.				//
//																						//
//				   Written at the University of Abertay Dundee, Scotland				//
//////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////
//	TERRAIN CLASS																		//
//	Handles the prototype's terrain landscape.											//
//////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////
//	CLASS HEADER
//////////////////////////////////////////////////////////////////////////////////////////
#include "Terrain.h"

//	Constructor.  Creates all required objects & data.  
//////////////////////////////////////////////////////////////////////////////////////////
TerrainGen::TerrainGen()
{
	// Creates the pointers for the two multitexturing functions.  
	glActiveTextureARB		= (PFNGLACTIVETEXTUREARBPROC)	wglGetProcAddress("glActiveTextureARB");
	glMultiTexCoord2fARB	= (PFNGLMULTITEXCOORD2FARBPROC)	wglGetProcAddress("glMultiTexCoord2fARB");

	this->LoadTexture();	// Loads in the grass texture.  
	this->GenerateBlank();	// Generates a blank landscape.  

	// Creates all otehr objects.  
	this->TerrainDL		= NULL;
	this->Sky			= new SkyDome(600.0f, 5.0f, 5.0f, 1.0f, 1.0f);
	this->TerrainShader	= new Shader();
	this->PerlinGen		= new FBMotion(8, 8, 2, 65536);
}

//	Destructor.
//////////////////////////////////////////////////////////////////////////////////////////
TerrainGen::~TerrainGen()
{
	glDeleteLists(this->TerrainDL, 1);	// Deletes the terrain's display list from memory.  
	
	delete Sky;							// Deletes the skydome.  
	delete TerrainShader;				// Deletes the terrain shader.  
}

//	Function to render the terrain.  .
//////////////////////////////////////////////////////////////////////////////////////////
void TerrainGen::Draw()
{
	// Renders the skydome.  
	glColor3f(1.0f, 1.0f, 1.0f);
	Sky->Render();

	// Renders the terrain.  
	TerrainShader->On(this->grass);			// Switches the terrain shader on.  
	glBegin(GL_QUADS);
		glCallList(this->TerrainDL);		// Calls the terrain's pre-generated display list
	glEnd();
	TerrainShader->Off();					// Switches the terrain shader off.  

	// Renders the sea plane.  
	glDisable(GL_TEXTURE_2D);				// Disables texturing for the sea plane.  
	glBegin(GL_QUADS);
		glColor3f(0.2f, 0.2f, 0.5f);
		glNormal3f(0.0f, 1.0f, 0.0f);
		glVertex3f(-512.0f, 0.0f, -512.0f);
		glVertex3f( 512.0f, 0.0f, -512.0f);
		glVertex3f( 512.0f, 0.0f,  512.0f);
		glVertex3f(-512.0f, 0.0f,  512.0f);
	glEnd();
	glEnable(GL_TEXTURE_2D);				// Enables texturing for all other objects to be rendered.  
}

//	Function to generate a blank texture.  
//////////////////////////////////////////////////////////////////////////////////////////
void TerrainGen::GenerateBlank()
{
	for EACH(v)
	{
		for EACH(u)
		{
			Node[u][v].Position	= GLVertex	(u - LIMIT	, 0.0f, v - LIMIT	);
			Node[u][v].Texture	= GLVertex2	(u / 4.0f	, v / 4.0f			);
		}
	}
	// Performs post-processing updates of normals & creates the new display list.  
	this->Update(0);
}

//	Function to generate a landscape based on Fractional Brownian Motion & Perlin Noise.  
//////////////////////////////////////////////////////////////////////////////////////////
void TerrainGen::GeneratePerlinNoise(int passes)
{
	// Runs a number of passes over the terrain.  Each pass consists of an additive FBH mask.  
	for (int i = 0 ; i < passes ; i++)
	{
		for EACH(v)
		{
			for EACH(u)
			{
				// For each node, randomise the seed and take a sample for each vertex in the landscape.  
				PerlinGen->RandomSeed(65536);
				Node[u][v].Position.y += PerlinGen->Get(Node[u][v].Position.x / LENGTH, 
														Node[u][v].Position.z / LENGTH);
			}
		}
	}
	// Performs post-processing updates of normals & creates the new display list.  
	this->Update(0);
}

//	Function to generate a landscape in the form of a bell curve
//////////////////////////////////////////////////////////////////////////////////////////
void TerrainGen::GenerateBellCurve()
{
	for EACH(v)
	{
		for EACH(u)
		{
			// For each vertex in the landscape, measures the length between the vertex's position 
			// and the center of the landscape.  
			GLVertex Length = GLVertex(Node[u][v].Position.x, 0.0f, Node[u][v].Position.z) 
							- GLVertex(Node[LIMIT][LIMIT].Position.x, 0.0f, Node[LIMIT][LIMIT].Position.z);
			GLfloat l		= Length.Length();

			// Creates a bell curve using a modified version of the function detailed in
			// http://www.willamette.edu/~mjaneba/help/normalcurve.html
			// Various values have been modified to change the shape of the bell curve.  
			GLfloat effect = exp(-(pow(l / 64.0f, 2.0f))) / pow(2 * PI, 0.5f);
			
			// Multiplies the calculated effect by 100.  
			Node[u][v].Position.y = effect * 100.0f;
			// Reduces the island slightly to create a smoother coastline.  
			Node[u][v].Position.y -= 0.1f;
		}
	}
	// Performs post-processing updates of normals & creates the new display list.  
	this->Update(0);
}

//	Function to create an island out of an existing landscape.  The island is generated by
//	a multiplicative bell-curve mask.  
//////////////////////////////////////////////////////////////////////////////////////////
void TerrainGen::IslandifyByBellCurve()
{
	int low_y = 0.0f;		// The lowest point on the landscape.  

	// First pass finds the lowest point on the landscape.  
	for EACH(v)
	{
		for EACH(u)
		{
			if (Node[u][v].Position.y < low_y)
				low_y = Node[u][v].Position.y;
		}
	}

	// Second pass adds this amount to all vertices so that the landscape is set to 0.0f at
	// its lowest.  
	for EACH(v)
	{
		for EACH(u)
			Node[u][v].Position.y -= low_y;
	}

	// Third pass applies the multiplicative bell curve mask.  
	for EACH(v)
	{
		for EACH(u)
		{
			// For each vertex in the landscape, measures the length between the vertex's position 
			// and the center of the landscape.  
			GLVertex Length = GLVertex(Node[u	 ][v	].Position.x, 0.0f, Node[u	  ][v	 ].Position.z) 
							- GLVertex(Node[LIMIT][LIMIT].Position.x, 0.0f, Node[LIMIT][LIMIT].Position.z);
			GLfloat l		= Length.Length();

			// Creates a bell curve using a modified version of the function detailed in
			// http://www.willamette.edu/~mjaneba/help/normalcurve.html
			// Various values have been modified to change the shape of the bell curve.  
			GLfloat effect = exp(-(pow(l / 125.0f, 2.0f)));

			Node[u][v].Position.y *= effect;		// Applies the mask.  
			Node[u][v].Position.y -= 0.5f;			// Also reduces all vertices by a minimal amount to produce more random coastlines.  
		}
	}
	this->Update(0);
}

//	Function to find the y-value of a particular point on the landscape.  Used for the FPS
//	camera.  
//////////////////////////////////////////////////////////////////////////////////////////
GLfloat TerrainGen::GetY(GLVertex Camera)
{
	// Finds the position of the nearest node to the camera.  
	GLint u = floor(Camera.x + 256.0f);
	GLint v = floor(Camera.z + 256.0f);

	// Finds the position of the node in relation to that node.  
	GLfloat node_x = fmod(Camera.x, 1.0f);
	GLfloat node_z = fmod(Camera.z, 1.0f);

	// Finds the component heights by calculating the heights between two adjacent nodes.  
	// Uses the equation V = v1 + r(v2 - v1)
	GLfloat spot_x = Node[u][v].Position.y + (node_x * (Node[u + 1][v].Position.y - Node[u][v].Position.y));
	GLfloat spot_z = Node[u][v].Position.y + (node_z * (Node[u][v + 1].Position.y - Node[u][v].Position.y));

	return (spot_x + spot_z) / 2.0f;	// Returns the average of the two y-values.  
}

//	Function to perform post-generation updates to the landscape.  
//////////////////////////////////////////////////////////////////////////////////////////
void TerrainGen::Update(int n)
{
	// Applies n number of shadng masks.  
	for (int i = 0 ; i < n ; i++)
		this->Smooth();

	this->GenerateNormals();		// Generates the required numbers.  
	this->GenerateDisplayList();	// Generates the resultant display list.  
}

//	Function to generate the intended display list.  
//////////////////////////////////////////////////////////////////////////////////////////
void TerrainGen::GenerateDisplayList()
{
	// If the display list already exists, delete it.  
	if (!this->TerrainDL)
		glDeleteLists(this->TerrainDL, 1);

	this->TerrainDL = glGenLists(1);			// Generate a new display list.  
	glNewList(this->TerrainDL, GL_COMPILE);		// Starts compilation of the new display list.  

	// For every quad in the landscape, the four vertices are rendered with the relevant texturing coords.  
	for (int v = 0 ; v < RESOLUTION ; v++)
	{
		for (int u = 0 ; u < RESOLUTION ; u++)
		{
			glMultiTexCoord2fARB(GL_TEXTURE0_ARB, Node[u    ][v    ].Texture.x, Node[u    ][v    ].Texture.y);
			Node[u    ][v    ].Draw();

			glMultiTexCoord2fARB(GL_TEXTURE0_ARB, Node[u + 1][v    ].Texture.x, Node[u + 1][v    ].Texture.y);
			Node[u + 1][v    ].Draw();

			glMultiTexCoord2fARB(GL_TEXTURE0_ARB, Node[u + 1][v + 1].Texture.x, Node[u + 1][v + 1].Texture.y);
			Node[u + 1][v + 1].Draw();

			glMultiTexCoord2fARB(GL_TEXTURE0_ARB, Node[u    ][v + 1].Texture.x, Node[u    ][v + 1].Texture.y);
			Node[u    ][v + 1].Draw();
		}
	}
	glEndList();
}

//	Function to generate the normals for the landscape.  
//////////////////////////////////////////////////////////////////////////////////////////
void TerrainGen::GenerateNormals()
{
	for EACH(v)
	{
		for EACH(u)
		{
			GLVertex vA, vB;		// Temporary normal vectors.  

			// vA = South adjacent vector - North adjacent vector.  If near the edge, imaginary vectors are used.  
			switch (u)
			{
				case 0:
					vA = Node[u + 1][v].Position - GLNode(-((RESOLUTION >> 1) + 1.0f), 0.0f, v).Position;
					break;
				case RESOLUTION:
					vA = GLNode((RESOLUTION >> 1) + 1.0f, 0.0f, v).Position - Node[u - 1][v].Position;
					break;
				default:
					vA = Node[u + 1][v].Position - Node[u - 1][v].Position;
					break;
			}	

			// vB = East adjacent vector - West adjacent vector.  If near the edge, imaginary vectors are used.  
			switch (v)
			{
				case 0:
					vB = Node[u][v + 1].Position - GLNode(u, 0.0f, -((RESOLUTION >> 1) + 1.0f)).Position;
					break;
				case RESOLUTION:
					vB = GLNode(u, 0.0f, (RESOLUTION >> 2)).Position - Node[u][v - 1].Position;
					break;
				default:
					vB = Node[u][v + 1].Position - Node [u][v - 1].Position;
					break;
			}
			
			// Normalises the first two temporary normals.  
			vA.Normalise();
			vB.Normalise();

			// Creates the normal as the cross product between the two temporary normals.  
			Node[u][v].Normal = vB.Cross(vA);
		}
	}
}

//	Function to smooth the landscape.  
//////////////////////////////////////////////////////////////////////////////////////////
void TerrainGen::Smooth()
{
	for EACH(v)
	{
		for EACH(u)
		{
			// Resets the two counters.  
			this->adjacent_sum = 0.0f;
			this->adjacent_blocks = 0;

			// Checks all adjacent nodes for heights.  
			GetAdjacentNode(u - 1, v - 1);
			GetAdjacentNode(u, v - 1);
			GetAdjacentNode(u + 1, v - 1);
			GetAdjacentNode(u + 1, v);
			GetAdjacentNode(u + 1, v + 1);
			GetAdjacentNode(u, v + 1);
			GetAdjacentNode(u - 1, v + 1);
			GetAdjacentNode(u - 1, v);

			// New y-position is the average of all adjacent nodes.  
			Node[u][v].Position.y = (this->adjacent_sum / this->adjacent_blocks);
		}
	}
	// Performs post-processing updates of normals & creates the new display list.  
	this->Update(0);
}

//	Function to get the height for an adjacent node.  
//////////////////////////////////////////////////////////////////////////////////////////
void TerrainGen::GetAdjacentNode(GLint u, GLint v)
{
	// If the target node is an edge or corner node, bail out.  
	if ((u < 0) || (u > RESOLUTION) || (v < 0) || (v > RESOLUTION))
		return;

	// Otherwise, add the height to the sum and add one more to the blocks list.  
	this->adjacent_sum += Node[u][v].Position.y;
	this->adjacent_blocks++;
}

//	Function to load the terrain texture.  DevIL is used in this function.  
//////////////////////////////////////////////////////////////////////////////////////////
int TerrainGen::LoadTexture()
{
	ILuint texid;		// DevIL Texture ID.  

	ilInit();			// Initialises DevIL.
	
	// Generates & binds a DevIL image name.  
	ilGenImages(1, &texid);
	ilBindImage(texid);

	// Loads the image.  
	if ((ilLoadImage("textures/grass11.jpg")) && ilConvertImage(IL_RGB, IL_UNSIGNED_BYTE))
	{
		// If the image was loaded correctly...
		glActiveTextureARB(GL_TEXTURE0_ARB);			// Creates Texture Unit 0.
		glEnable(GL_TEXTURE_2D);						// Enable texturing.  
		glGenTextures(1, &this->grass);					// Create The Texture
		glBindTexture(GL_TEXTURE_2D, this->grass);		// Binds the texture.  
	    glTexImage2D(	GL_TEXTURE_2D, 0, ilGetInteger(IL_IMAGE_BPP), ilGetInteger(IL_IMAGE_WIDTH),
						ilGetInteger(IL_IMAGE_HEIGHT), 0, ilGetInteger(IL_IMAGE_FORMAT), GL_UNSIGNED_BYTE,
						ilGetData());
		
		// Sets linear filtering.  
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);	
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}

	// Deletes the DevIL image now that a copy has been uploaded to memory.  
	ilDeleteImages(1, &texid);

	return true;								// Return The Status
}