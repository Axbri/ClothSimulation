#pragma once

#include "particle.h"
#include "camera.h"
#include "loader.h"
#include "shader.h"
#include "vec3.h"

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
	void updateNormals(); 
	static const int NUMBER_OF_VERTICES = 30;
	Particle particles[NUMBER_OF_VERTICES][NUMBER_OF_VERTICES];
	Model clothModel; 
	Loader theLoader; 
	Shader shader;
};

