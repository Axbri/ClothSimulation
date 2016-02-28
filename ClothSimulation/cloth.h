#pragma once

#include "particle.h"
#include "camera.h"
#include "loader.h"
#include "shader.h"
#include "vec3.h"
#include "light.h"
#include <vector>
#include "sphere.h"

class Cloth
{
public:
	Cloth(Loader loader, Vec3 pos, double size, double totalWeight);
	~Cloth();

	void setMode(bool useContraints); 
	void reset(); 
	void update(double delta_time, double time, Sphere sphere);
	void render(GLFWwindow* window, Camera camera, vector<Light> allLights);
	void cleanUp(); 

private: 
	void updateVBOs();
	void updateNormals(); 
	void resolve_constraint(int x, int y);
	void calculate_force(int x, int y, double k[]);
	void verlet(int x, int y, double step, double damping, Vec3 g);
	void collision(int x, int y, Sphere sphere);
	void reset_forces();

	static const int NUMBER_OF_VERTICES = 34;
	Particle particles[NUMBER_OF_VERTICES][NUMBER_OF_VERTICES];

	Model clothModel; 
	GLuint contraintsTexture, massSpringTexture; 
	Loader theLoader; 
	Shader shader;
	double time_passed = 0;
	double restlength;
	double size;
	Vec3 position;	// world position, this is loaded into the model matrix. 
	double useContraints; 

	
};

