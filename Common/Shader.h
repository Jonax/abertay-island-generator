//////////////////////////////////////////////////////////////////////////////////////////
//						  Dynamic Terrain Generation Prototype							//
//					  Written 2007 by Jon Wills (jc@chromaticaura.net)						//
//				Written for a Win32 environment using the DirectSound API.				//
//																						//
//				   Written at the University of Abertay Dundee, Scotland				//
//////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////
//	CG SHADER CLASS CODE																//
//	Used to utilise shaders written in the nVidia Cg shader language.					//
//////////////////////////////////////////////////////////////////////////////////////////

#ifndef _SHADER_H_
#define _SHADER_H_

//////////////////////////////////////////////////////////////////////////////////////////
//	EXTRA INCLUDE FILES
//////////////////////////////////////////////////////////////////////////////////////////
#include <windows.h>			// Win32 library.  
#include "include/Cg/Cg.h"		// Cg language library.  
#include "include/Cg/cgGL.h"	// Cg library for OpenGL applications.  

//////////////////////////////////////////////////////////////////////////////////////////
//	CLASS DECLARATIONS
//	Declarations of the class and its members.  
//////////////////////////////////////////////////////////////////////////////////////////
class Shader
{
	public:
		Shader();					// Class constructor.  
		~Shader();					// Class destructor.  

		void On(GLuint texture);	// Switches the shader on with a specific texture.  
		void Off();					// Switches the shader off.  

	private:
		void CreateProfiles();		// Switches the required Cg profiles.  
		void LoadPrograms();		// Loads the required Cg programs.  
		void SetParameters();		// Sets parameters used in the shaders.  

	//////////////////////////////////////////////////////////////////////////////////////
	//	CLASS OBJECTS
	//////////////////////////////////////////////////////////////////////////////////////
		CGcontext	CGContext;		// Primary GPU context for Cg.  
		CGprofile	PixelProfile;	// GPU's profile.  
		CGprogram	PixelProgram;	// Program for the pixel shader.  
		
		CGparameter	Texture;		// Parameter for the texture to use in the shader.  
};

#endif