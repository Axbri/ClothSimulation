#pragma once

#include "loader.h"
#include "shader.h"

using namespace std;

class Font
{
public:
	Font(Loader loader, double size);

	void setColor(double r, double g, double b);
	void render(double number, double xPos, double yPos);
	void render(int number, double xPos, double yPos);
	void render(char *str, double xPos, double yPos);
	void cleanUp(); 

private: 

	double Font::getCharWidth(char c);

	GLuint texture; 
	GLuint vao;
	float fontSize; 
	float red, green, blue; // color
	float xPos, yPos;		// position
	Shader fontShader;
};

