//////////////////////////////////////////////////////////////////////////////////////////
//						  Dynamic Terrain Generation Prototype							//
//					  Written 2007 by Jon Wills (jc@chromaticaura.net)						//
//				Written for a Win32 environment using the DirectSound API.				//
//																						//
//				   Written at the University of Abertay Dundee, Scotland				//
//////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////
//	PERLIN NOISE CODE																	//
//	Used as a random generator for the creation of the terrain.							//
//																						//
//	NOTE - This code is taken straight out of a code snippet available at the address	//
//	below and is used as is.  Changes made are the seperation of the original class		//
//	into Perlin Noise & Fractional Brownian Motion, and the creation of the SetSeed()	//
//	method, which sets a new seed for the Perlin generator.								//
//																						//
//	http://www.flipcode.com/cgi-bin/fcarticles.cgi?show=64126							//
//////////////////////////////////////////////////////////////////////////////////////////

#ifndef _PERLIN_H_
#define _PERLIN_H_

#include <stdlib.h>
#include <math.h>

#define SAMPLE_SIZE	1024
#define B			SAMPLE_SIZE
#define BM			(SAMPLE_SIZE - 1)

#define N			0x1000
#define NP 12		/* 2^N */
#define NM			0xfff

#define s_curve(t)		(t * t * (3.0f - 2.0f * t))
#define lerp(t, a, b)	(a + t * (b - a))
#define at2(a, b)		(rx[a] * q[0] + ry[b] * q[1])
#define at3(rx,ry,rz)	(rx * q[0] + ry * q[1] + rz * q[2])

#define setup(i, b, r)\
					t	= vec[i] + N;\
					b[0]	= ((int)t) & BM;\
					b[1]	= (b[0]+1) & BM;\
					r[0]	= t - (int)t;\
					r[1]	= r[0] - 1.0f;

class Perlin
{
	public:
		Perlin();
		Perlin(int Seed);

		void SetSeed(int Seed);

		float Noise1(float vec[1]);
		float Noise2(float vec[2]);
		float Noise3(float vec[3]);

	private:
		void Initialise();
		void Normalize2(float v[2]);
		void Normalize3(float v[3]);

		int		 p[SAMPLE_SIZE + SAMPLE_SIZE + 2];
		float	g1[SAMPLE_SIZE + SAMPLE_SIZE + 2];
		float	g2[SAMPLE_SIZE + SAMPLE_SIZE + 2][2];
		float	g3[SAMPLE_SIZE + SAMPLE_SIZE + 2][3];

		int		mSeed;
		bool	mStart;
};

#endif