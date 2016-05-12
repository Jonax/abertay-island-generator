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

#ifndef _GLVERTEX_H_
#define _GLVERTEX_H_

//////////////////////////////////////////////////////////////////////////////////////////
//	LIBRARY INCLUDE
//	Refer to include.h for more details.
//////////////////////////////////////////////////////////////////////////////////////////
#include "Include.h"

//////////////////////////////////////////////////////////////////////////////////////////
//	EXTRA INCLUDE FILES
//////////////////////////////////////////////////////////////////////////////////////////
#include "GLVertex2.h"		// 2D Vector class.  

//////////////////////////////////////////////////////////////////////////////////////////
//	CLASS DECLARATIONS
//	Declarations of the GLVertex class and its members, as well as its inheritance from
//	the GLVertex2 class.  Refer to GLVertex.cpp for the code for each member and their 
//	in-depth explanations.  
//////////////////////////////////////////////////////////////////////////////////////////
class GLVertex : public GLVertex2
{
	public:
		GLVertex();									// Basic constructor to create a vertex at the origin.
		GLVertex(GLfloat x, GLfloat y, GLfloat z);	// Constructor to create a vertex at x, y, z.

		GLVertex & operator + (GLVertex &rhs);		// Operator to add the vertex to another vertex.
		GLVertex & operator - (GLVertex &rhs);		// Operator to subtract the vertex to another vertex.
		GLVertex & operator * (GLfloat f);			// Operator to multiply the vertex by a scale value.
		GLVertex & operator / (GLfloat f);			// Operator to divide the vertex by a scale value.

		void operator =	 (GLVertex &rhs);			// Operator to assign the vertex from another vertex.  
		void operator += (GLVertex &rhs);			// Operator to add to the vertex by another vertex.  
		void operator *= (GLfloat f);				// Operator to multiply to the vertex by a scale value.  
		void operator /= (GLfloat f);				// Operator to divide to the vertex by a scale value.  

		GLfloat		Length();						// Function to return the length of the vertex.  
		void		Normalise();					// Function to normalise the vertex.  
		GLfloat		Dot3(GLVertex& rhs);			// Function to return the vertex's 3D dot product.  
		GLVertex	Cross(GLVertex& rhs);			// Function to return the vertex's cross product.  

//////////////////////////////////////////////////////////////////////////////////////////
//	VERTEX CLASS DATA
//	An extra OpenGL-compliant float to hold the z co-ordinates.  
//////////////////////////////////////////////////////////////////////////////////////////
		GLfloat z;									// Float to store the z value.  
};

#endif