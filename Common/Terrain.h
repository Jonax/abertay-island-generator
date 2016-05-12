//////////////////////////////////////////////////////////////////////////////////////////
//						  Dynamic Terrain Generation Prototype							//
//					  Written 2007 by Jon Wills (jc@chromaticaura.net)						//
//				Written for a Win32 environment using the DirectSound API.				//
//																						//
//				   Written at the University of Abertay Dundee, Scotland				//
//////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////
//	TERRAIN CLASS																		//
//	Handles the prototype's terrain landscape.											//
//////////////////////////////////////////////////////////////////////////////////////////

#ifndef _TERRAIN_H_
#define _TERRAIN_H_

//////////////////////////////////////////////////////////////////////////////////////////
//	INCLUDE FILE
//////////////////////////////////////////////////////////////////////////////////////////
#include "Include.h"

//////////////////////////////////////////////////////////////////////////////////////////
//	EXTRA INCLUDE FILES
//////////////////////////////////////////////////////////////////////////////////////////
#include "Shader.h"			// Cg Shader class
#include "FBMotion.h"		// Fractional Brownian Motion class.  
#include "SkyDome.h"		// Skydome class
#include "TNode.h"			// Terrain Node class.  
#include "GLVertex.h"		// Main OpenGL Vertex class.

#define RESOLUTION	512										// 512x512 node landscape.  
#define LIMIT		(RESOLUTION >> 1)						// Limit is 256 (-256 to 256)
#define EACH(c)		(int c = 0 ; c <= RESOLUTION ; c++)		// Each node in the row.  
#define LENGTH		(RESOLUTION + 1)						// Length is 513.

//////////////////////////////////////////////////////////////////////////////////////////
//	CLASS DECLARATIONS
//	Declarations of the class and its members.  
//////////////////////////////////////////////////////////////////////////////////////////
class TerrainGen
{
	public:
		TerrainGen();			// Constructor.  
		~TerrainGen();			// Destructor.  

		void Draw();			// Function to render the terrain.  

		void GenerateBlank();					// Generates a blank landscape.  
		void GeneratePerlinNoise(int passes);	// Generates a landscape using FBM & Perlin Noise.  
		void GenerateBellCurve();				// Generates a bell curve.  
		void IslandifyByBellCurve();			// Creates a landscape with an existing landscape.  

		// Gets the y-value of a particular point on the landscape.  
		GLfloat GetY(GLVertex Camera);

	private:
		void Update(int n);				// Generates supplementary data after a sequence of passes.  
		void GenerateDisplayList();		// Generates a display list.  

		void GenerateNormals();			// Generates normals for a landscape.  
		void Smooth();					// Applies a pass to smooth the landscape.  

		void GetAdjacentNode(GLint u, GLint v);		// Gets the data for adjacent nodes.  
		int LoadTexture();							// Load the terrain's texture.  
		
	//////////////////////////////////////////////////////////////////////////////////////
	//	CLASS OBJECTS
	//////////////////////////////////////////////////////////////////////////////////////
		TerrainNode Node[RESOLUTION + 1][RESOLUTION + 1];	// Data for each vertex in the landscape.  
		FBMotion *PerlinGen;								// FBM & Perlin Noise Generator
		Shader* TerrainShader;								// Cg Shader Program.  
		SkyDome *Sky;										// Skydome.  

		PFNGLMULTITEXCOORD2FARBPROC		glMultiTexCoord2fARB;	// Pointer for the glMultiTexCoord2fARB command.  
		PFNGLACTIVETEXTUREARBPROC		glActiveTextureARB;		// Pointer for the glActiveTextureARB command.  

	//////////////////////////////////////////////////////////////////////////////////////
	//	CLASS DATA
	//////////////////////////////////////////////////////////////////////////////////////
		float adjacent_sum;		// Sum height of all nodes adjacent to a specific node.  
		int adjacent_blocks;	// Number of nodes adjacent to a specific node.  
		int TerrainDL;			// ID for the Terrain's display list.  

		GLuint grass;			// ID for the Terrain's grass texture.  
};

#endif