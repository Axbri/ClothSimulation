#include "light.h"

Light::Light()
{

}

Light::Light(double x, double y, double z)
{
	xPos = x; 
	yPos = y; 
	zPos = z; 
}

Light::~Light()
{

}

double Light::get_xPos()
{
	return xPos;
}

double Light::get_yPos()
{
	return yPos;
}

double Light::get_zPos()
{
	return zPos;
}

