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
	Vec3 pos_old;
	Vec2 textureCoord; 
	Vec3 normal; 

	double mass; 

private: 
	
};

