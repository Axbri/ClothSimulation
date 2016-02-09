#pragma once

#include <GL/glew.h> 
#include <GLFW/glfw3.h>  
#include <stdio.h>  
#include <stdlib.h> 
#include <iostream>
#include <sstream>

#include "mat4.h"
#include "vec4.h"
#include "vec3.h"
#include "vec2.h"

using namespace std;

class Shader
{
public:

	Shader();

	void createShader(char *vertexshaderfile, char *fragmentshaderfile);

	void setUniformMat4(char *variableName, float value[]);
	void setUniformMat4(char *variableName, Mat4 matrix);
	void setUniformVec4(char *variableName, float x1, float x2, float x3, float x4);
	void setUniformVec4(char *variableName, Vec4 vector);
	void setUniformVec3(char *variableName, float x1, float x2, float x3);
	void setUniformVec3(char *variableName, Vec3 vector);
	void setUniformVec2(char *variableName, float x1, float x2);
	void setUniformVec2(char *variableName, Vec2 vector);
	void setUniformFloat(char *variableName, float value);
	void setUniformInt(char *variableName, int value);
	void start();
	void stop(); 
	void cleanUp(); 


private: 

	long filelength(FILE *file);
	unsigned char* readShaderFile(const char *filename);

	GLuint thisShader; 

};