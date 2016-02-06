#pragma once

#include "camera.h"
#include "loader.h"
#include "shader.h"
#include "vec3.h"
#include "light.h"
#include <vector>

class Sphere
{
public:
	Sphere(double x, double y, double z, double r, Loader loader);
	void update(double delta_time);
	void render(GLFWwindow* window, Camera camera, vector<Light> allLights);
	void cleanUp();
	void setPos(double x, double y, double z); 

private: 
	static const int NUMBER_OF_SEGMENTS = 32;
	Vec3 position; 
	double radius; 
	Model sphereModel;
	Shader sphereShader;
};

