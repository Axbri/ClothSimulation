#pragma once
class Particle
{
public:
	Particle(); 
	Particle(double x, double y, double z, double u, double v, double m);
	~Particle();

	double xPos; 
	double yPos; 
	double zPos; 
	double uTexture; 
	double vTexture; 
	double xNormal;
	double yNormal; 
	double zNormal; 

	double mass; 

private: 
	
};

