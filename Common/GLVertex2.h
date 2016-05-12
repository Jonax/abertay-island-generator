//////////////////////////////////////////////////////////////////////////////////////////
//						  Dynamic Terrain Generation Prototype							//
//					  Written 2007 by Jon Wills (jc@chromaticaura.net)						//
//				Written for a Win32 environment using the DirectSound API.				//
//																						//
//				   Written at the University of Abertay Dundee, Scotland				//
//////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////
//	2D VERTEX CLASS CODE																//
//	Used as a means to hold primitive data for OpenGL vertices in a fashion where		//
//	mathematical calculations between vertices can be performed in a contained manner.	//
//////////////////////////////////////////////////////////////////////////////////////////

#ifndef _GLVERTEX_2_
#define _GLVERTEX_2_

//////////////////////////////////////////////////////////////////////////////////////////
//	LIBRARY INCLUDE FILE
//	Refer to include.h for more details.
//////////////////////////////////////////////////////////////////////////////////////////
#include "Include.h"

//////////////////////////////////////////////////////////////////////////////////////////
//	VERTEX CLASS DECLARATIONS
//	Declarations of the GLVertex2 class and its members.  Refer to GLVertex2.cpp for the 
//	code for each member and their in-depth explanations.  
//////////////////////////////////////////////////////////////////////////////////////////
class GLVertex2
{
	public:
		GLVertex2();								// Basic constructor to create a vertex at the origin.
		GLVertex2(GLfloat x, GLfloat y);			// Constructor to create a vertex at x, y.

		void operator = (const GLVertex2 &rhs);		// Operator to assign values based on GLVertex2.

//////////////////////////////////////////////////////////////////////////////////////////
//	VERTEX CLASS DATA
//	Two OpenGL-compliant floats to hold the co-ordinates for the vertex.  
//////////////////////////////////////////////////////////////////////////////////////////
		GLfloat x, y;								// Floats to hold vertex co-ordinates.  
};

#endif