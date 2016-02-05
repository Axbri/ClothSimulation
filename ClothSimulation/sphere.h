#pragma once

#include "camera.h"
#include "loader.h"
#include "shader.h"
#include "vec3.h"

class Sphere
{
public:
	Sphere(double x, double y, double z, double r, Loader loader);
	void render(GLFWwindow* window, Camera camera);
	void cleanUp();

private: 
	static const int NUMBER_OF_SEGMENTS = 32;
	Vec3 position; 
	double radius; 
	Model sphereModel;
	Shader sphereShader;
};

