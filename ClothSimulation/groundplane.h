#pragma once

#include "camera.h"
#include "loader.h"
#include "shader.h"
#include "light.h"
#include <vector>

class GroundPlane
{
public:
	GroundPlane(Loader loader);
	void render(GLFWwindow* window, Camera camera, vector<Light> allLights);
	void cleanUp();

private: 
	Model groundModel;
	GLuint normalMapTexture; 
	Shader groundShader;
};

