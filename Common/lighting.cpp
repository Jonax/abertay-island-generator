#include "include.h"
#include "lighting.h"

Light::Light(	GLshort id,
				float amb_x, float amb_y, float amb_z, float amb_a, 
				float dif_x, float dif_y, float dif_z, float dif_a,
				float pos_x, float pos_y, float pos_z)
{
	Assign(id);
	SetAmbient(amb_x, amb_y, amb_z, amb_a);
	SetDiffuse(dif_x, dif_y, dif_z, dif_a);
	SetPosition(pos_x, pos_y, pos_z);
}

Light::~Light()
{
	Disable();
}

void Light::Assign(GLshort id)
{
	light_id	= id;
}

void Light::SetAmbient(float x, float y, float z, float a)
{
	Ambient[0]	= x;
	Ambient[1]	= y;
	Ambient[2]	= z;
	Ambient[3]	= a;
}

void Light::SetDiffuse(float x, float y, float z, float a)
{
	Diffuse[0]	= x;
	Diffuse[1]	= y;
	Diffuse[2]	= z;
	Diffuse[3]	= a;
}

void Light::SetPosition(float x, float y, float z)
{
	Position[0]	= x;
	Position[1]	= y;
	Position[2]	= z;
	Position[3] = 1.0f;
}

void Light::Enable()
{
	glLightfv(light_id, GL_AMBIENT, Ambient);
	glLightfv(light_id, GL_DIFFUSE, Diffuse);
	glLightfv(light_id, GL_POSITION, Position);
	glEnable(light_id);
}

void Light::Disable()
{
	glDisable(light_id);
}