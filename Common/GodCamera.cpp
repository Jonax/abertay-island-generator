//////////////////////////////////////////////////////////////////////////////////////////
//						  Dynamic Terrain Generation Prototype							//
//					  Written 2007 by Jon Wills (jc@chromaticaura.net)						//
//				Written for a Win32 environment using the DirectSound API.				//
//																						//
//				   Written at the University of Abertay Dundee, Scotland				//
//////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////
//	GOD CAMERA CLASS																	//
//	Handles the prototype's god camera.													//
//////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////
//	CLASS HEADER
//////////////////////////////////////////////////////////////////////////////////////////
#include "GodCamera.h"

//	Constructor.  Sets up basic details and creates the attached camera.  
//////////////////////////////////////////////////////////////////////////////////////////
GodCamera::GodCamera()
{
	this->angle			= 0.0f;		// Sets the rotation angle as 0.
	this->zoom			= 0.0f;		// Sets the zoom scale as default.  

	this->rotate_lock	= true;		// Sets the rotation as initially locked.  
	this->zoom_lock		= true;		// Sets the zoom as initially locked.  

	// Sets the camera at the edge of the island area, 50 units above sea level.  
	Camera = new GLCamera(	GLVertex(0.0f, 50.0f, -256.0f),
							GLVertex(0.0f, 0.0f, 0.0f));
}

//	Destructor.  
//////////////////////////////////////////////////////////////////////////////////////////
GodCamera::~GodCamera()
{
	delete this->Camera;
}

//	Function to set the OpenGL camera to that of the god camera.  
//////////////////////////////////////////////////////////////////////////////////////////
void GodCamera::Look()
{
	// Sets the camera at a position in a circle around the island.  
	Camera->Position = GLVertex(cos(this->angle) * 256.0f, 50.0f, sin(this->angle) * 256.0f);
	this->Zoom();			// Applies the zoom scale.  
	Camera->Look();			// Applies the current coordinates to the camera.  
}

//	Function to apply the zoom scale to the camera.  
//////////////////////////////////////////////////////////////////////////////////////////
void GodCamera::Zoom()
{
	// Gets the camera's direction vector.  
	GLVertex Direction = Camera->LookAt - Camera->Position;

	// Camera position = Position + (Direction Vector * Zoom Scale)
	Camera->Position += (Direction * this->zoom);
}

//	Function to process input from the mouse.  
//////////////////////////////////////////////////////////////////////////////////////////
void GodCamera::ProcessMouseInput(POINT MousePos, POINT OldMouse)
{
	// If the rotation lock is off, rotate the camera by the difference in the mouse's x
	// position between the current & last frame.  
	if (!this->rotate_lock)
		this->angle += (MousePos.x - OldMouse.x) * 0.01f;
	// If the zoom lock is off, zoom the camera by the difference in the mouse's y position 
	// between the current & last frame.  
	if (!this->zoom_lock)
		this->zoom += (MousePos.y - OldMouse.y) * -0.01f;

	// Clamps the zoom scale so that it exists between -0.5 and 0.5.
	if (this->zoom >= 0.5f)
		this->zoom = 0.5f;
	if (this->zoom <= -0.5f)
		this->zoom = -0.5f;
}

//	Function to set the rotation lock.  
//////////////////////////////////////////////////////////////////////////////////////////
void GodCamera::SetRotate(bool lock)
{
	this->rotate_lock = lock;
}

//	Function to set the zoom lock.  
//////////////////////////////////////////////////////////////////////////////////////////
void GodCamera::SetZoom(bool lock)
{
	this->zoom_lock = lock;
}