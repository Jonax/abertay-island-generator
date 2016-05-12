//////////////////////////////////////////////////////////////////////////////////////////
//						  Dynamic Terrain Generation Prototype							//
//					  Written 2007 by Jon Wills (jc@chromaticaura.net)						//
//				Written for a Win32 environment using the DirectSound API.				//
//																						//
//				   Written at the University of Abertay Dundee, Scotland				//
//////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////
//	FIRST PERSON CAMERA CLASS															//
//	Handles the prototype's FPS camera.													//
//////////////////////////////////////////////////////////////////////////////////////////

#ifndef _FPSCAMERA_H_
#define _FPSCAMERA_H_

//////////////////////////////////////////////////////////////////////////////////////////
//	EXTRA INCLUDE FILES
//////////////////////////////////////////////////////////////////////////////////////////
#include "GLCamera.h"		// OpenGL Camera class.  

//////////////////////////////////////////////////////////////////////////////////////////
//	CLASS DECLARATIONS
//	Declarations of the class and its members.  
//////////////////////////////////////////////////////////////////////////////////////////
class FPSCamera
{
	public:
		FPSCamera();		// Constructor.  
		~FPSCamera();		// Destructor.  

		void Look();		// Sets the OpenGL camera to use this camera.  
		void ProcessMouseInput(POINT MousePos, POINT OldMouse);		// Processes mouse input.  

		void MoveCamera(float amount);		// Moves the camera.  
		void StrafeCam(float amount);		// Strafes the camera.  

		void SetHeight(float new_y);		// Sets the height to follow the contour.  

	//////////////////////////////////////////////////////////////////////////////////////
	//	CLASS OBJECTS
	//////////////////////////////////////////////////////////////////////////////////////
		GLCamera *Camera;					// OpenGL camera class.
};

#endif