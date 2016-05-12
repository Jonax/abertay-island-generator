//////////////////////////////////////////////////////////////////////////////////////////
//						  Dynamic Terrain Generation Prototype							//
//					  Written 2007 by Jon Wills (jc@chromaticaura.net)						//
//				Written for a Win32 environment using the DirectSound API.				//
//																						//
//				   Written at the University of Abertay Dundee, Scotland				//
//////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////
//	VERTEX CLASS CODE																	//
//	Used as a means to hold primitive data for OpenGL vertices in a fashion where		//
//	mathematical calculations between vertices can be performed in a contained manner.	//
//////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////
//	VERTEX CLASS HEADER
//	Links to the GLVertex's declarations.  
//////////////////////////////////////////////////////////////////////////////////////////
#include "GLVertex.h"		// Main OpenGL Vertex class.

//	Generic constructor.  Creates a vertex at (0, 0, 0).
//////////////////////////////////////////////////////////////////////////////////////////
GLVertex::GLVertex()
: GLVertex2(0.0f, 0.0f)
{
	this->z = 0.0f;
}

//	Constructor.  Creates a vertex at (x, y, z).
//////////////////////////////////////////////////////////////////////////////////////////
GLVertex::GLVertex(GLfloat x, GLfloat y, GLfloat z)
: GLVertex2(x, y)
{
	this->z = z;
}

// Operator to add the vertex by another vertex.
//////////////////////////////////////////////////////////////////////////////////////////
GLVertex & GLVertex::operator + (GLVertex &rhs)
{
	return GLVertex(this->x + rhs.x, 
					this->y + rhs.y,
					this->z + rhs.z);
}

// Operator to subtract the vertex from another vertex.
//////////////////////////////////////////////////////////////////////////////////////////
GLVertex & GLVertex::operator - (GLVertex &rhs)
{
	return GLVertex(this->x - rhs.x, 
					this->y - rhs.y,
					this->z - rhs.z);
}

// Operator to multiply the vertex by a scale value.
//////////////////////////////////////////////////////////////////////////////////////////
GLVertex & GLVertex::operator * (float f)
{
	return GLVertex(this->x * f, 
					this->y * f,
					this->z * f);
}

// Operator to divide the vertex by a scale value.
//////////////////////////////////////////////////////////////////////////////////////////
GLVertex & GLVertex::operator / (float f)
{
	return GLVertex(this->x / f, 
					this->y / f,
					this->z / f);
}

// Operator to assign the vertex from another vertex.  
//////////////////////////////////////////////////////////////////////////////////////////
void GLVertex::operator = (GLVertex &rhs)
{
	this->x = rhs.x;
	this->y = rhs.y;
	this->z = rhs.z;
}

// Operator to add to the vertex by another vertex.  
//////////////////////////////////////////////////////////////////////////////////////////
void GLVertex::operator += (GLVertex &rhs)
{
	this->x += rhs.x;
	this->y += rhs.y;
	this->z += rhs.z;
}

// Operator to multiply to the vertex by a scale value.  
//////////////////////////////////////////////////////////////////////////////////////////
void GLVertex::operator *= (float f)
{
	this->x *= f;
	this->y *= f;
	this->z *= f;
}

// Operator to divide to the vertex by a scale value.  
//////////////////////////////////////////////////////////////////////////////////////////
void GLVertex::operator /= (float f)
{
	this->x /= f;
	this->y /= f;
	this->z /= f;
}

// Function to return the length of the vertex.  
//////////////////////////////////////////////////////////////////////////////////////////
float GLVertex::Length()
{
	return (sqrt(	(this->x * this->x)
				 +	(this->y * this->y)
				 +	(this->z * this->z)));
}

// Function to normalise the vertex.  
//////////////////////////////////////////////////////////////////////////////////////////
void GLVertex::Normalise()
{
	float f = this->Length();

	this->x = this->x / f;
	this->y = this->y / f;
	this->z = this->z / f;
}

// Function to return the vertex's 3D dot product.  
//////////////////////////////////////////////////////////////////////////////////////////
GLfloat GLVertex::Dot3(GLVertex& rhs)
{
	return ((this->x * rhs.x) +
			(this->y * rhs.y) + 
			(this->z * rhs.z));
}

// Function to return the vertex's cross product. 
//////////////////////////////////////////////////////////////////////////////////////////
GLVertex GLVertex::Cross(GLVertex& rhs)
{
	return GLVertex(	(this->y * rhs.z) - (this->z * rhs.y),
						(this->z * rhs.x) - (this->x * rhs.z), 
						(this->x * rhs.y) - (this->y * rhs.x));
}