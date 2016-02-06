#pragma once

#include "vec3.h"
#include <vector>
#include "shader.h"

class Light
{
public:
	Light();
	Light(double xPos, double yPos, double zPos);
	Vec3 pos;
	Vec3 color; 
	Vec3 attenuation; 

	static void loadLightsToShader(Shader shader, vector<Light> allLights); 
};

