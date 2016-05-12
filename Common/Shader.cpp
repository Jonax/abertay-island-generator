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

//////////////////////////////////////////////////////////////////////////////////////////
//	CLASS HEADER
//////////////////////////////////////////////////////////////////////////////////////////
#include "Shader.h"

//	Constructor.  Sets up the shader for later use.  
//////////////////////////////////////////////////////////////////////////////////////////
Shader::Shader()
{
	this->CGContext = cgCreateContext();	// Create the context for the Cg program.  

	this->CreateProfiles();					// Creates the required Cg profiles.  
	this->LoadPrograms();					// Loads the required Cg programs.  
	this->SetParameters();					// Sets the needed parameters for the shader.  
}

//	Destructor.  
//////////////////////////////////////////////////////////////////////////////////////////
Shader::~Shader()
{
	cgDestroyContext(this->CGContext);		// Destroys the context after use.  
}

//	Function to switch on the shader.  The ID for an OpenGL texture is used to render the
//	texture.  
//////////////////////////////////////////////////////////////////////////////////////////
void Shader::On(GLuint texture)
{
	cgGLSetTextureParameter(this->Texture, texture);	// Sets the texture for the shader.  

	cgGLEnableProfile(	this->PixelProfile);	// Enables the Profile for the pixel shader.  
	cgGLBindProgram(	this->PixelProgram);	// Binds the pixel shader program.  

	cgGLEnableTextureParameter(this->Texture);	// Enables the Texture parameter.  
}

//	Function to switch off the shader after use.  
//////////////////////////////////////////////////////////////////////////////////////////
void Shader::Off()
{
	cgGLDisableTextureParameter(this->Texture);	// Disables the Texture parameter.  

	cgGLDisableProfile(this->PixelProfile);		// Disables the Pixel shader profile.  
}

//	Function to create all required shader profiles.  
//////////////////////////////////////////////////////////////////////////////////////////
void Shader::CreateProfiles()
{
	// Gets the latest profile that the GPU can run.  
	this->PixelProfile	= cgGLGetLatestProfile(CG_GL_FRAGMENT);

	// If a valid Cg profile can't be found, bail out.  
	if (this->PixelProfile == CG_PROFILE_UNKNOWN)
	{
		MessageBox(NULL, "Invalid pixel profile type", "Error", MB_OK);
		PostQuitMessage(0);
	}

	// Otherwise, get the optimal options for the pixel shader's profile.  
	cgGLSetOptimalOptions(this->PixelProfile);
}

//	Function to load all required shader programs.  
//////////////////////////////////////////////////////////////////////////////////////////
void Shader::LoadPrograms()
{
	// Creates the CG pixel shader from the .cg file.  
	this->PixelProgram	= cgCreateProgramFromFile(this->CGContext, CG_SOURCE, "pixel_shader.cg", this->PixelProfile,	NULL, NULL);

	// If the pixel shader couldn't be loaded, time to throw out an error.  
	if (!this->PixelProgram)
	{
		// Gets the needed error message.  
		CGerror Error = cgGetError();

		// Show the error in a Win32 message box and bail out.  
		MessageBox(NULL, cgGetErrorString(Error), "Error", MB_OK);
		PostQuitMessage(0);
	}

	// Loads the program into memory to be called later.  
	cgGLLoadProgram(this->PixelProgram);
}

//	Function to set parameters used in the shaders.  
//////////////////////////////////////////////////////////////////////////////////////////
void Shader::SetParameters()
{
	// Passes the current texture to the pixel shader.  
	this->Texture		= cgGetNamedParameter(this->PixelProgram, "sTexture");
}