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

	void updateModelMatrix();
	void render(GLFWwindow* window, Camera camera, vector<Light> allLights);
	void cleanUp();
	void setPos(Vec3 pos);
	Vec3 getPos();
	double getRadius();

private: 
	static const int NUMBER_OF_SEGMENTS = 32;
	Vec3 position; 
	double radius; 
	Model sphereModel;
	Shader sphereShader;
};

