#pragma once

#include "particle.h"
#include "camera.h"
#include "loader.h"
#include "shader.h"

class Cloth
{
public:
	Cloth(Loader loader, double size, double totalWeight);
	~Cloth();

	void update(double delta_time, double time);
	void render(GLFWwindow* window, Camera camera);
	void cleanUp(); 

private: 
	void updateVBOs();

	static const int NUMBER_OF_VERTICES = 48;
	Particle particles[NUMBER_OF_VERTICES][NUMBER_OF_VERTICES];
	Model clothModel; 
	Loader theLoader; 
	Shader shader;
};

