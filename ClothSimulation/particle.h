#pragma once

#include "vec3.h"
#include "vec2.h"

class Particle
{
public:
	Particle(); 
	Particle(double x, double y, double z, double u, double v, double m);
	~Particle();

	Vec3 pos; 
	Vec3 pos_old;			// denna kanske kan vara private? den anv�nds ju inte utanf�r denna klassen?
	Vec2 textureCoord; 
	Vec3 normal; 

	double mass; 

private: 
	
};

