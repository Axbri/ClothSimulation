#include "particle.h"

Particle::Particle()
{
	pos = Vec3(0, 0, 0);
	pos_old = Vec3(pos);
	force = Vec3(0, 0, 0);
	textureCoord.set(0, 0);
	normal = Vec3(0, 0, 0);
	mass = 1;
}

Particle::Particle(double x, double y, double z, double u, double v, double m)
{
	pos = Vec3(x, y, z);
	pos_old = Vec3(pos);
	force = Vec3(0, 0, 0);
	textureCoord.set(u, v);
	normal = Vec3(0, 0, 1);
	mass = m; 
}


Particle::~Particle()
{

}
