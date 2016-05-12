//////////////////////////////////////////////////////////////////////////////////////////
//						  Dynamic Terrain Generation Prototype							//
//					  Written 2007 by Jon Wills (jc@chromaticaura.net)						//
//				Written for a Win32 environment using the DirectSound API.				//
//																						//
//				   Written at the University of Abertay Dundee, Scotland				//
//////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////
//	FRACTIONAL BROWNIAN MOTION CLASS CODE												//
//	Used to process Perlin Noise to generate random heights for a terrain.				//
//////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////
//	CLASS HEADER
//////////////////////////////////////////////////////////////////////////////////////////
#include "FBMotion.h"

//	Constructor.  Creates the Perlin generator and sets the stats of the FBM generator.  
//////////////////////////////////////////////////////////////////////////////////////////
FBMotion::FBMotion(int octaves, float freq, float amp, int range)
{
	// Creates the Perlin Noise generator.  
	this->PerlinGen = new Perlin();

	// Sets the various stats.  
	this->SetOctaves	(octaves);
	this->SetFrequency	(freq);
	this->SetAmplitude	(amp);
	this->RandomSeed	(range);
}

//	Function to get a random value based on a set of 2D coordinates.  
//////////////////////////////////////////////////////////////////////////////////////////
float FBMotion::Get(float x, float y)
{
	float vec[2];		// Standard array for Perlin Noise generator compliance.  

	// Fills the array with the parameters.  
	vec[0] = x;
	vec[1] = y;

	// Returns the value returned from the PN generator based on the 2D coords.  
	return this->PerlinNoise2D(vec);
};

//	Function to set the number of octaves used in the FBM generation.  
//////////////////////////////////////////////////////////////////////////////////////////
void FBMotion::SetOctaves(float octaves)
{
	this->mOctaves		= octaves;
}

//	Function to set the frequency used in the FBM generation.  
//////////////////////////////////////////////////////////////////////////////////////////
void FBMotion::SetFrequency(float frequency)
{
	this->mFrequency	= frequency;
}

//	Function to set the amplitude used in the FBM generation.  
//////////////////////////////////////////////////////////////////////////////////////////
void FBMotion::SetAmplitude(float amplitude)
{
	this->mAmplitude	= amplitude;
}

//	Function to set a random seed based on a specific range.  Seed is created between
//	0 - range.  
//////////////////////////////////////////////////////////////////////////////////////////
void FBMotion::RandomSeed(int range)
{
	srand(GetTickCount());

	PerlinGen->SetSeed(rand() % range);
}

//	Function to generate a random value using the 2D version of the Perlin noise generator.  
//////////////////////////////////////////////////////////////////////////////////////////
float FBMotion::PerlinNoise2D(float vec[2])
{
	float result	= 0.0f;					// Result set as 0.  
	float amp		= this->mAmplitude;		// Current amplitude set to that of the saved amplitude.  

	// Each input coord is multiplied by the frequency.  
	for (int n = 0 ; n < 2 ; n++)
		vec[n] *= this->mFrequency;

	// For however many octaves are specified, a pass with Perlin is performed.  
	for (int i = 0 ; i < this->mOctaves ; i++)
	{
		// Result is increased by the PNGen's result multiplied by the current amplitude.  
		result	+= PerlinGen->Noise2(vec) * amp;

		// Each coord is multiplied by 2.  
		for (int n = 0 ; n < 2 ; n++)
			vec[n] *= 2.0f;

		// Amplitude is halved.  
		amp *= 0.5f;
	}

	// Once done, return the result.  
	return result;
}