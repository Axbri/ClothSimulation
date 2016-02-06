#include "particle.h"

Particle::Particle()
{
	pos.set(0, 0, 0);
	pos_old.set(pos);
	textureCoord.set(0, 0);
	normal.set(0, 0, 0);
	mass = 1;
}

Particle::Particle(double x, double y, double z, double u, double v, double m)
{
	pos.set(x, y, z);
	pos_old.set(pos);
	textureCoord.set(u, v);
	normal.set(0, 0, 1);
	mass = m; 
}


Particle::~Particle()
{

}
