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
	static const int H_NUMBER_OF_VERTICES = 40;
	static const int V_NUMBER_OF_VERTICES = 20;
	Vec3 position; 
	double radius; 
	Model sphereModel;
	Shader sphereShader;
};

