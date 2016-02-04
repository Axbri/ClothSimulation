#include "particle.h"



Particle::Particle()
{
	xPos = 0;
	yPos = 0;
	zPos = 0;
	uTexture = 0;
	vTexture = 0;
	xNormal = 0;
	yNormal = 0;
	zNormal = 1;
	mass = 1;
}

Particle::Particle(double x, double y, double z, double u, double v, double m)
{
	xPos = x; 
	yPos = y; 
	zPos = z; 
	uTexture = u;
	vTexture = v;
	xNormal = 0;
	yNormal = 0;
	zNormal = 1;
	mass = m; 
}


Particle::~Particle()
{

}
