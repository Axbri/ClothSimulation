#pragma once

#include "loader.h"
#include "shader.h"
#include "vec3.h"
#include "vec2.h"
#include <iomanip>

using namespace std;

class Font
{
public:
	Font(Loader loader, double size, double aspectRatio);

	void setColor(double r, double g, double b);
	void render(char *name, Vec2 vec, double xPos, double yPos);
	void render(char *name, Vec3 vec, double xPos, double yPos);
	void render(char *name, double number, double xPos, double yPos);
	void render(char *name, int number, double xPos, double yPos);
	void render(char *str, double xPos, double yPos);
	void cleanUp(); 

private: 

	double Font::getCharWidth(char c);

	double aspectRatio; 
	GLuint texture; 
	GLuint vao;
	float fontSize; 
	float red, green, blue; // color
	float xPos, yPos;		// position
	Shader fontShader;
};

