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
//	The majority of this code is taken from the Skydome demo by Luis R. Semp� of Sphere
//	games.  All code within functions is preserved as written by Semp�, all other
//	modifications are commented.  
//
//	http://www.spheregames.com/index.php?p=templates/pages/tutorials
//////////////////////////////////////////////////////////////////////////////////////////

#ifndef _SKYDOME_H_
#define _SKYDOME_H_

//////////////////////////////////////////////////////////////////////////////////////////
//	INCLUDE FILE
//////////////////////////////////////////////////////////////////////////////////////////
#include "Include.h"

//////////////////////////////////////////////////////////////////////////////////////////
//	EXTRA INCLUDE FILES
//////////////////////////////////////////////////////////////////////////////////////////
#include <math.h>			// Maths library.  
#include "GLNode.h"			// OpenGL Node class.

//////////////////////////////////////////////////////////////////////////////////////////
//	CLASS DECLARATIONS
//	Declarations of the class and its members.  
//////////////////////////////////////////////////////////////////////////////////////////
class SkyDome
{
	public:
		// Constructor.  
		SkyDome(float radius, float dtheta, float dphi, float hTile, float vTile);
		~SkyDome();			// Destructor.  

		void Render();		// Function for rendering the skysphere.  

	private:
		// Function to generate the dome. 
		void GenerateDome(float radius, float dtheta, float dphi, float hTile, float vTile);
		void LoadTexture();				// Loads the skysphere's texture.  

		void GenerateDisplayList();		// Generates the OpenGL display list.  
		void CheckSeam(int a, int b);	// Checks the seam of the skysphere for mismatching.  

	//////////////////////////////////////////////////////////////////////////////////////
	//	CLASS OBJECTS
	//////////////////////////////////////////////////////////////////////////////////////
		// Pointer for the glMultiTexCoord2fARB command.  
		PFNGLMULTITEXCOORD2FARBPROC		glMultiTexCoord2fARB;
		// Pointer for the glActiveTextureARB command.
		PFNGLACTIVETEXTUREARBPROC		glActiveTextureARB;

		GLNode *Vertices;				// Dynamic list of vertices for the skydome.  

	//////////////////////////////////////////////////////////////////////////////////////
	//	CLASS DATA
	//////////////////////////////////////////////////////////////////////////////////////
		GLuint NumVertices;				// Number of vertices for the skydome.  
		GLuint sky;						// ID for the sky texture.  
		GLuint SkyDL;					// ID for the sky display list.  

};

#endif