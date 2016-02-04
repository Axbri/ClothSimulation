#pragma once

#include "camera.h"
#include "loader.h"
#include "shader.h"
#include "vec3.h"

class Sphere
{
public:
	Sphere(double x, double y, double z, int numberOfSegments, Loader loader);
	void render(GLFWwindow* window, Camera camera);
	void cleanUp();

private: 
	Vec3 position; 
	double radius; 
	Model sphereModel;
	Shader sphereShader;
};

