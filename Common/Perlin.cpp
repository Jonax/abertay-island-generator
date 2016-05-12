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

#include "Perlin.h"

Perlin::Perlin()
{
	this->mStart		= true;
}

Perlin::Perlin(int Seed)
{
	this->SetSeed(Seed);

	this->mStart		= true;
}

void Perlin::SetSeed(int Seed)
{
	this->mSeed = Seed;
}

float Perlin::Noise1(float vec[1])
{
	int bx[2];
	float rx[2], sx, t, u, v;

	if (this->mStart)
	{
		srand(this->mSeed);
		this->mStart = false;
		this->Initialise();
	}

	setup(0, bx, rx);

	sx = s_curve(rx[0]);

	u = rx[0] * this->g1[p[bx[0]]];
	v = rx[1] * this->g1[p[bx[1]]];

	return lerp(sx, u, v);
}

float Perlin::Noise2(float vec[2])
{
	int bx[2], by[2], b[2][2];
	float rx[2], ry[2], *q, sx, sy, c, d, t, u, v;
	int i, j;

	if (this->mStart)
	{
		srand(this->mSeed);
		mStart = false;
		this->Initialise();
	}

	setup(0, bx, rx);
	setup(1, by, ry);

	i = p[bx[0]];
	j = p[bx[1]];

	b[0][0] = p[i + by[0]];
	b[1][0] = p[j + by[0]];
	b[0][1] = p[i + by[1]];
	b[1][1] = p[j + by[1]];

	sx = s_curve(rx[0]);
	sy = s_curve(ry[0]);

	q = this->g2[b[0][0]]	;	u = at2(0, 0);
	q = this->g2[b[1][0]]	;	v = at2(1, 0);
	c = lerp(sx, u, v);

	q = this->g2[b[0][1]]	;	u = at2(0, 1);
	q = this->g2[b[1][1]]	;	v = at2(1, 1);
	d = lerp(sx, u, v);

	return lerp(sy, c, d);
}

float Perlin::Noise3(float vec[3])
{
	int bx[2], by[2], bz[2], b[2][2];
	float rx[2], ry[2], rz[2], *q, sy, sz, c, d, e, f, t, u, v;
	int i, j;

	if (this->mStart)
	{
		srand(this->mSeed);
		this->mStart = false;
		this->Initialise();
	}

	setup(0, bx, rx);
	setup(1, by, ry);
	setup(1, bz, rz);

	i = p[bx[0]];
	j = p[bx[1]];

	b[0][0] = p[i + by[0]];
	b[1][0] = p[j + by[0]];
	b[0][1] = p[i + by[1]];
	b[1][1] = p[j + by[1]];

	t  = s_curve(rx[0]);
	sy = s_curve(ry[0]);
	sz = s_curve(rz[0]);

	q = g3[b[0][0] + bz[0]] ; u = at3(rx[0], ry[0], rz[0]);
	q = g3[b[1][0] + bz[0]] ; v = at3(rx[1], ry[0], rz[0]);
	c = lerp(t, u, v);

	q = g3[b[0][1] + bz[0]] ; u = at3(rx[0], ry[1], rz[0]);
	q = g3[b[1][1] + bz[0]] ; v = at3(rx[1], ry[1], rz[0]);
	d = lerp(t, u, v);

	e = lerp(sy, c, d);

	q = g3[b[0][0] + bz[1]] ; u = at3(rx[0], ry[0], rz[0]);
	q = g3[b[1][0] + bz[1]] ; v = at3(rx[1], ry[0], rz[0]);
	c = lerp(t, u, v);

	q = g3[b[0][1] + bz[1]] ; u = at3(rx[0], ry[1], rz[1]);
	q = g3[b[1][1] + bz[1]] ; v = at3(rx[1], ry[1], rz[1]);
	d = lerp(t, u, v);

	f = lerp(sy, c, d);

	return lerp(sz, e, f);
}

void Perlin::Initialise()
{
	int i, j, k;

	for (i = 0 ; i < B ; i++)
	{
		p[i] = i;
		g1[i] = (float) ((rand() % (B + B)) - B) / B;
		
		for (j = 0 ; j < 2 ; j++)
			g2[i][j] = (float)((rand() % (B + B)) - B) / B;
		this->Normalize2(g2[i]);

		for (j = 0 ; j < 3 ; j++)
			g3[i][j] = (float)((rand() % (B + B)) - B) / B;
		this->Normalize3(g3[i]);
	}

	while (--i)
	{
		k = p[i];
		p[i] = p[j = rand() % B];
		p[j] = k;
	}

	for (i = 0 ; i < B + 2 ; i++)
	{
		p[B + i] = p[i];
		g1[B + i] = g1[i];

		for (j = 0 ; j < 2 ; j++)
			g2[B + i][j] = g2[i][j];
		for (j = 0 ; j < 3 ; j++)
			g3[B + i][j] = g3[i][j];
	}
}

void Perlin::Normalize2(float v[2])
{
	float s;

	s = (float) sqrt(v[0] * v[0] + v[1] * v[1]);
	s = 1.0f / s;

	for (int i = 0 ; i < 2 ; i++)
		v[i] *= s;
}

void Perlin::Normalize3(float v[3])
{
	float s;

	s = (float) sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
	s = 1.0f / s;

	for (int i = 0 ; i < 3 ; i++)
		v[i] *= s;
}