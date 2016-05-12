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

#ifndef _OPENGL_H_
#define _OPENGL_H_

//////////////////////////////////////////////////////////////////////////////////////////
//	INCLUDE FILE
//////////////////////////////////////////////////////////////////////////////////////////
#include "Include.h"

//////////////////////////////////////////////////////////////////////////////////////////
//	EXTRA INCLUDE FILES
//////////////////////////////////////////////////////////////////////////////////////////
#include "Singleton.h"		// Singleton pattern class.  
#include "GodCamera.h"		// God-type camera class.  
#include "FPSCamera.h"		// First-Person camera class.  
#include "Terrain.h"		// Terrain Landscape class.  

//	Client area resolution - 800 x 600
#define S_WIDTH		800
#define S_HEIGHT	600

// Declaration of the singleton'd class.  
#define Renderer OpenGL::GetSingleton()

//////////////////////////////////////////////////////////////////////////////////////////
//	CLASS DECLARATIONS
//	Declarations of the class and its members.  
//////////////////////////////////////////////////////////////////////////////////////////
class OpenGL : public CSingleton<OpenGL>
{
	public:
		OpenGL();				// Constructor.  
		~OpenGL();				// Destructor.  

		void DrawScene();		// Renders the prototype's scene.  

		void InitializeOpenGL(RECT gRect);			// Creates the OpenGL instance.  
		void Initialise(HWND hwnd);					// Sets up key OpenGL elements.  
		void ResizeGLWindow(int width, int height);	// Resizes the OpenGL window as required.  

		void Cleanup();				// Destroys OpenGL instance during application shutdown.  

		void MoveCamera(float amount);			// Moves the camera relative to the camera's bearing.  
		void StrafeCam(float amount);			// Strafes the camera relative to the camera's bearing.  
		void ProcessMouseInput(POINT MousePos);	// Processes input from the mouse.  
		void ResetCamera(POINT MousePos);		// Sets the stored mouse position to the current position.  

	private:
		bool SetupPixelFormat(HDC hdc);	// Sets up the required Pixel Format.  
		void SetTerrainHeight();		// Sets the height of the FPS camera in relation to the terrain.  

	//////////////////////////////////////////////////////////////////////////////////////
	//	CLASS OBJECTS
	//////////////////////////////////////////////////////////////////////////////////////
	private:
		HWND        ghwnd;				// Win32 handle.  
		HDC			ghdc;
		HGLRC		ghrc;

		FPSCamera*	FPSCam;				// Prototype's First Person camera.  
	public:
		GodCamera*	GodCam;				// Prototype's God camera.  
		TerrainGen*	Terrain;			// Terrain Landscape

		RECT		gRect;				// Rectangle for OpenGL window.  
		POINT		NewMouse, OldMouse;	// Mouse positions for calculating differences.  

	//////////////////////////////////////////////////////////////////////////////////////
	//	CLASS DATA
	//////////////////////////////////////////////////////////////////////////////////////
		bool god_mode;					// Boolean for selecting camera mode (GodCam or FPSCam).
};

#endif