//////////////////////////////////////////////////////////////////////////////////////////
//						  Dynamic Terrain Generation Prototype							//
//					  Written 2007 by Jon Wills (jc@chromaticaura.net)						//
//				Written for a Win32 environment using the DirectSound API.				//
//																						//
//				   Written at the University of Abertay Dundee, Scotland				//
//////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////
//	CAMERA CLASS HEADER																	//
//////////////////////////////////////////////////////////////////////////////////////////
//	IMPORTANT NOTE
//	A significant amount of the code below is taken from the project submitted in May 2006
//	as the project for Graphics Programming.  As a result, the class remains in the same
//	state it was in then - All new elements are commented thoroughly.  
//////////////////////////////////////////////////////////////////////////////////////////
#include "GLCamera.h"

GLCamera::GLCamera()
{
	this->SetCamera(GLVertex(0.0f, 0.0f, 1.0f), 
					GLVertex(0.0f, 0.0f, 0.0f));
	this->AngleRotation = 0.0f;
}

GLCamera::GLCamera(GLVertex Position, GLVertex LookAt)
{
	this->SetCamera(Position, LookAt);
}

void GLCamera::Look()
{
	gluLookAt(	Position.x, Position.y, Position.z,
				LookAt.x, LookAt.y, LookAt.z,
				0.0f, 1.0f, 0.0f);
}

void GLCamera::SetCamera(GLVertex Position, GLVertex LookAt)
{
	this->Position	= Position;
	this->LookAt	= LookAt;
}

// NEW FOR 2007
// Function to calculate the movement of the camera on an X-Z plane.  The function is
// derived from the MoveCamera() method below - The only difference is the removal of the
// effect on the camera's y coord - This allows another part of the program to affect the
// y value.  
//////////////////////////////////////////////////////////////////////////////////////////
void GLCamera::MoveCameraXZ(GLfloat direction)
{
	GLVertex LookDir;

	LookDir = LookAt - Position;

	LookDir.Normalise();

	Position.x += LookDir.x * direction;
	Position.z += LookDir.z * direction;

	LookAt.x += LookDir.x * direction;
	LookAt.z += LookDir.z * direction;
}

void GLCamera::MoveCamera(GLfloat direction)
{
   // Moving the camera requires a little more then adding 1 to the z or subracting 1.
   // First we need to get the direction at which we are looking.
	GLVertex LookDir;

	// The look direction is the view (where we are looking) minus the position (where we are).
	LookDir = LookAt - Position;

   // Normalize the direction.
	LookDir.Normalise();

   // To correctly move the camera foward and backwards we simply add the result of the
   // look direction * the speed of our camera to get the new position.  This will move the
   // camera but what about the view?  If the view was set to look at the point (0, 0, 0)
   // then when we move the camera forward or backward the camera's view will stay
   // focused on that one point.  We don't want that so we also effect the view data.

   // Move the camera on the X and Z axis.
	Position.x += LookDir.x * direction;
	Position.y += LookDir.y * direction;
	Position.z += LookDir.z * direction;

   // Move the view along with the position
	LookAt.x += LookDir.x * direction;
	LookAt.y += LookDir.y * direction;
	LookAt.z += LookDir.z * direction;
}

void GLCamera::StrafeCam(float direction)
{
   CalculateStrafe();                           // Calculate the straft direction.

   // Move the camera on the X and Z axis.
	Position.x += Strafe.x * direction;
	Position.z += Strafe.z * direction;

   // Move the view along with the position
	LookAt.x += Strafe.x * direction;
	LookAt.z += Strafe.z * direction;
}

void GLCamera::CalculateStrafe()
{
	GLVertex StrafeDir;

   // Strafing is just like moving the camera forward and backward.  First we will get the
   // direction we are looking.
	StrafeDir = LookAt - Position;

   // Normalize the direction.
	StrafeDir.Normalise();

   // Now if we were to call UpdateCamera() we will be moving the camera foward or backwards.
   // We don't want that here.  We want to strafe.  To do so we have to get the cross product
   // of our direction and Up direction view.  The up was set in SetCamera to be 1 positive
   // y.  That is because anything positive on the y is considered up.  After we get the
   // cross product we can save it to the strafe variables so that can be added to the
   // camera using UpdateCamera().

   // Get the cross product of the direction we are looking and the up direction.
	// Save our strafe (cross product) values in xStrafe, yStrafe, and zStrafe.
	Strafe = StrafeDir.Cross(GLVertex(0.0f, 1.0f, 0.0f));
}

void GLCamera::RotateCamera(float AngleDir, GLVertex Speed)
{
/* QUATERNION BASED CODE 
	GLQuaternion Rotation, View, NewView;

	// Create the rotation quaternion based on the axis we are rotating on.
	Rotation.Rotate(AngleDir, Speed.x, Speed.y, Speed.z);

	// Create the view quaternion.  This will be the direction of the view and position.
	View.x = LookAt.x - Position.x;
	View.y = LookAt.y - Position.y;
	View.z = LookAt.z - Position.z;
	View.w = 0.0f;

   // Create the resulting quaternion by multiplying the rotation quat by the view quat
   // then multiplying that by the conjugate of the rotation quat.
	NewView = ((Rotation * View) * Rotation.Conjugate());

	// Update the view information by adding the position to the resulting quaternion.
	LookAt.x = Position.x + NewView.x;
	LookAt.y = Position.y + NewView.y;
	LookAt.z = Position.z + NewView.z;
*/

	GLVertex LookDir, NewLookDir;

   // First we will need to calculate the cos and sine of our angle.  I creaetd two macros to
   // do this in the CCamera.h header file called GET_COS and GET_SINE.  To use the macros
   // we just send in the variable we ant to store the results and the angle we need to
   // calculate.
	float CosAngle = (float) cos(AngleDir);
	float SinAngle = (float) sin(AngleDir);

	// Next get the look direction (where we are looking) just like in the move camera function.
	LookDir = LookAt - Position;

   // Normalize the direction.
	LookDir.Normalise();

	// Calculate the new X position.
	NewLookDir.x = (CosAngle + (1 - CosAngle) * Speed.x) * LookDir.x;
	NewLookDir.x += ((1 - CosAngle) * Speed.x * Speed.y - Speed.z * SinAngle)* LookDir.y;
	NewLookDir.x += ((1 - CosAngle) * Speed.x * Speed.z + Speed.y * SinAngle) * LookDir.z;

	// Calculate the new Y position.
	NewLookDir.y = ((1 - CosAngle) * Speed.x * Speed.y + Speed.z * SinAngle) * LookDir.x;
	NewLookDir.y += (CosAngle + (1 - CosAngle) * Speed.y) * LookDir.y;
	NewLookDir.y += ((1 - CosAngle) * Speed.y * Speed.z - Speed.x * SinAngle) * LookDir.z;

	// Calculate the new Z position.
	NewLookDir.z = ((1 - CosAngle) * Speed.x * Speed.z - Speed.y * SinAngle) * LookDir.x;
	NewLookDir.z += ((1 - CosAngle) * Speed.y * Speed.z + Speed.x * SinAngle) * LookDir.y;
	NewLookDir.z += (CosAngle + (1 - CosAngle) * Speed.z) * LookDir.z;

	// Last we add the new rotations to the old view to correctly rotate the camera.
	LookAt = Position + NewLookDir;
}

void GLCamera::RotateByMouse(int mousePosX, int mousePosY, int midX, int midY)
{
	float yDirection = 0.0f;         // Direction angle.
	float yRotation = 0.0f;          // Rotation angle.

	// If the mouseX and mouseY are at the middle of the screen then we can't rotate the view.
	if ((mousePosX == midX) && (mousePosY == midY))
		return;

	// Next we get the direction of each axis.  We divide by 1000 to get a smaller value back.
	yDirection = (float)((midX - mousePosX)) / 200.0f;		
	yRotation = (float)((midY - mousePosY)) / 200.0f;		

	// We use curentRotX to help use keep the camera from rotating too far in either direction.
	AngleRotation -= yRotation;  

	// Stop the camera from going to high...
	//if(AngleRotation > 1.0f)
	//{
	//	AngleRotation = 1.0f;
	//	return;
	//}

	//// Stop the camera from going to low...
	//if(AngleRotation < -1.0f)
	//{
	//	AngleRotation = -1.0f;
	//	return;
	//}

   // Next we get the axis which is a perpendicular vector of the view direction and up values.
   // We use the cross product of that to get the axis then we normalize it.
	GLVertex Axis, LookDir;

   // Get the Direction of the view.
	LookDir = LookAt - Position;

   // Get the cross product of the direction and the up.
	Axis = LookDir.Cross(GLVertex(0.0f, 1.0f, 0.0f));

   // Normalize it.
	Axis.Normalise();

   // Rotate the camera.
   RotateCamera(yRotation, Axis);
   RotateCamera(yDirection, GLVertex(0.0f, 1.0f, 0.0f));
}