#pragma once

#include "camera.h"
#include "loader.h"
#include "shader.h"

class GroundPlane
{
public:
	GroundPlane(Loader loader);
	void render(GLFWwindow* window, Camera camera);	
	void cleanUp();

private: 
	Model groundModel;
	GLuint normalMapTexture; 
	Shader groundShader;
};

