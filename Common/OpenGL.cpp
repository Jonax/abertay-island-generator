//////////////////////////////////////////////////////////////////////////////////////////
//						  Dynamic Terrain Generation Prototype							//
//					  Written 2007 by Jon Wills (jc@chromaticaura.net)						//
//				Written for a Win32 environment using the DirectSound API.				//
//																						//
//				   Written at the University of Abertay Dundee, Scotland				//
//////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////
//	OPENGL RENDERER																		//
//	The main processing class, handling the majority of the prototype's rendering.		//
//////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////
//	CLASS HEADER
//////////////////////////////////////////////////////////////////////////////////////////
#include "OpenGL.h"

//////////////////////////////////////////////////////////////////////////////////////////
//	CLASS GLOBAL
//////////////////////////////////////////////////////////////////////////////////////////
OpenGL Renderer_SingletonInit;		// Initialises the singleton pattern within the class.  

//	Constructor.  
//////////////////////////////////////////////////////////////////////////////////////////
OpenGL::OpenGL()
{
	this->god_mode = true;			// Sets the starting camera mode as God Camera.  
}

//	Destructor.  
//////////////////////////////////////////////////////////////////////////////////////////
OpenGL::~OpenGL()
{
}

//	Function to render the prototype's scene.  
//////////////////////////////////////////////////////////////////////////////////////////
void OpenGL::DrawScene()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// Clears the screen & depth buffer.
	glLoadIdentity();									// Loads the Identity Matrix.  

	// Look through a camera depending on which camera mode is active.  
	if (this->god_mode)
		GodCam->Look();
	else
		FPSCam->Look();

	// Basic information for a positional light.  
	GLfloat Light_Ambient[4]	= {0.6f, 0.6f, 0.6f, 1.0f};
	GLfloat Light_Diffuse[4]	= {1.0f, 1.0f, 1.0f, 1.0f};
	GLfloat Light_Position[4]	= {0.0f, 40.0f, 0.0f, 1.0f};

	// Passes the light data to memory.  
	glLightfv(GL_LIGHT1, GL_AMBIENT,  Light_Ambient);
	glLightfv(GL_LIGHT1, GL_DIFFUSE,  Light_Diffuse);
	glLightfv(GL_LIGHT1, GL_POSITION, Light_Position);
	// Enables GL Light 1.
	glEnable(GL_LIGHT1);

	Terrain->Draw();			// Renders the terrain landscape.  

	SwapBuffers(ghdc);			// Swaps the frame buffers.
}

//	Function to initialise the OpenGL instance.  
//////////////////////////////////////////////////////////////////////////////////////////
void OpenGL::InitializeOpenGL(RECT gRect) 
{  
    ghdc = GetDC(ghwnd);			// Sets the global HDC.  

    if (!SetupPixelFormat(ghdc))	// Sets the pixel format.
        PostQuitMessage(0);

    ghrc = wglCreateContext(ghdc);	// Creates rendering contextfrom HDC.
    wglMakeCurrent(ghdc, ghrc);		// Uses this HRC.

	this->ResizeGLWindow(gRect.right, gRect.bottom);	// Sets up the Screen.
}

//	Function to set up OpenGL elements.  
//////////////////////////////////////////////////////////////////////////////////////////
void OpenGL::Initialise(HWND hwnd)
{
	this->ghwnd = hwnd;
	GetClientRect(this->ghwnd, &gRect);					// Get rect into our handy global rect
	this->InitializeOpenGL(gRect);						// Initialises OpenGL.  

	//OpenGL settings
	glShadeModel(GL_SMOOTH);							// Enables Smooth Shading
	glClearColor(0.0f, 0.0f, 0.0f, 0.5f);				// Black Background
	glClearDepth(1.0f);									// Depth Buffer Setup
	glEnable(GL_DEPTH_TEST);							// Enables Depth Testing
	glDepthFunc(GL_LEQUAL);								// The Type Of Depth Testing To Do
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	// Really Nice Perspective Calculations
	glEnable(GL_TEXTURE_2D);							// Enables 2D texturing.  
	glEnable(GL_LIGHTING);								// Enables Lighting.  

	// Creates the terrain landscape.  
	Terrain	= new TerrainGen();
	// Creates the two cameras.  
	GodCam	= new GodCamera();
	FPSCam	= new FPSCamera();

	GetCursorPos(&OldMouse);
}

//	Function to resize the OpenGL Window.  
//////////////////////////////////////////////////////////////////////////////////////////
void OpenGL::ResizeGLWindow(int width, int height)
{
	// {Prevents a "Divide By Zero" error.  
	if (height == 0)
		height = 1;

	// Sets the viewport.  
	glViewport(0, 0, width, height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	// Calculates the aspect ratio
	gluPerspective(45.0f, (GLfloat)width/height, 1, 1024.0f);

	glMatrixMode(GL_MODELVIEW);		// Selects The Modelview Matrix
	glLoadIdentity();				// Resets The Modelview Matrix
}

//	Function to clean up after application shutdown.  
//////////////////////////////////////////////////////////////////////////////////////////
void OpenGL::Cleanup()
{
	if (ghrc)
	{
		wglMakeCurrent(NULL, NULL);		// Frees rendering memory
		wglDeleteContext(ghrc);			// Deletes OpenGL Rendering Context
	}

	if (ghdc)
		ReleaseDC(ghwnd, ghdc);			// Releases HDC from memory
}

//	Function to move the First Person camera based on its bearing.  
//////////////////////////////////////////////////////////////////////////////////////////
void OpenGL::MoveCamera(float amount)
{
	// If the First Person camera is set... 
	if (!this->god_mode)
	{
		FPSCam->MoveCamera(amount);		// Moves the camera around.
		this->SetTerrainHeight();		// Sets the terrain height seperately.  
	}
}

//	Function to strafe the First Person camera based on its bearing.  
//////////////////////////////////////////////////////////////////////////////////////////
void OpenGL::StrafeCam(float amount)
{
	// If the First Person camera is set, strafe the camera.  
	if (!this->god_mode)
		FPSCam->StrafeCam(amount);
}

//	Function to send the current frame's mouse information to the active camera.  
//////////////////////////////////////////////////////////////////////////////////////////
void OpenGL::ProcessMouseInput(POINT MousePos)
{
	if (this->god_mode)
		GodCam->ProcessMouseInput(MousePos, this->OldMouse);
	else
		FPSCam->ProcessMouseInput(MousePos, this->OldMouse);
}

//	Function to update the stored camera position to the current camera position.  Called
//	at the end.  
//////////////////////////////////////////////////////////////////////////////////////////
void OpenGL::ResetCamera(POINT MousePos)
{
	this->OldMouse.x = MousePos.x;
	this->OldMouse.y = MousePos.y;
}

//	Function to set up the pixel format.  
//////////////////////////////////////////////////////////////////////////////////////////
bool OpenGL::SetupPixelFormat(HDC hdc) 
{ 
    PIXELFORMATDESCRIPTOR pfd; 
    int pixelformat; 
 
    pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);	// Sets the size of the structure
    pfd.nVersion = 1;

	// Passs in the appropriate OpenGL flags
    pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER; 
    pfd.dwLayerMask = PFD_MAIN_PLANE;			// Standard mask (this is ignored anyway)
    pfd.iPixelType = PFD_TYPE_RGBA;				// RGB and Alpha pixel type
    pfd.cColorBits = COLOUR_DEPTH;				// Here we use our #define for the color bits
    pfd.cDepthBits = COLOUR_DEPTH;				// Ignored for RBA
    pfd.cAccumBits = 0;							// nothing for accumulation
    pfd.cStencilBits = 0;						// nothing for stencil
 
	// Gets a best match on the pixel format as passed in from device
    if ((pixelformat = ChoosePixelFormat(hdc, &pfd)) == false) 
    { 
        MessageBox(NULL, "ChoosePixelFormat failed", "Error", MB_OK); 
        return false; 
    } 
 
	// Sets the pixel format if it's OK. 
    if (SetPixelFormat(hdc, pixelformat, &pfd) == false) 
    { 
        MessageBox(NULL, "SetPixelFormat failed", "Error", MB_OK); 
        return false; 
    }
 
    return true;
}

//	Function to set the camera's position to follow the terrain.  
//////////////////////////////////////////////////////////////////////////////////////////
void OpenGL::SetTerrainHeight()
{
	// Gets a new camera height based on the current position of the camera.  
	GLVertex CamPos = FPSCam->Camera->Position;
	float new_y = Terrain->GetY(CamPos);

	new_y += 10.0f;				// Camera height is increased by 10.0f.  
	FPSCam->SetHeight(new_y);	// Camera position is set to the new position.  
}