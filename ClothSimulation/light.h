#pragma once

#include "vec3.h"

class Light
{
public:
	Light();
	Light(double xPos, double yPos, double zPos);
	Vec3 pos;
};

