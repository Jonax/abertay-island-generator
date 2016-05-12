#ifndef _LIGHTING_H_
#define _LIGHTING_H_

#include "include.h"

class Light
{
	public:
		Light(	GLshort id,
				float amb_x, float amb_y, float amb_z, float amb_a, 
				float dif_x, float dif_y, float dif_z, float dif_a,
				float pos_x, float pos_y, float pos_z);
		~Light();

		void Assign		(GLshort id);
		void SetAmbient	(float x, float y, float z, float a);
		void SetDiffuse	(float x, float y, float z, float a);
		void SetPosition(float x, float y, float z);
		
		void Enable();
		void Disable();

	private:
		int light_id;

		GLfloat Ambient[4];
		GLfloat Diffuse[4];
		GLfloat Position[4];
};

#endif