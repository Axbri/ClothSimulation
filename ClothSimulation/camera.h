#pragma once

#include <GL/glew.h> 
#include <algorithm>
#include "matrixmath.h"
#include "userinput.h"

using namespace std;

class Camera
{
public:
	Camera(float aspectRatio);
	void update(double delta_time);
	void getViewMatrix(GLfloat matrix[]);
	void getProjectionMatrix(GLfloat matrix[]);

private:
	void updateViewMatrix(); 
	double xPos;
	double yPos;
	double zPos;
	double distance;
	double orbitAngle;
	double tiltAngle;
	GLfloat viewMatrix[16];
	GLfloat projMatrix[16];

	const double MOUSE_ROTATION_SENSITIVITY = 0.015;
	const double DOLLY_SENSITIVITY = 0.1;
	const double MIN_DOLLY = 0.5;
	const double MAX_DOLLY = 10;
};