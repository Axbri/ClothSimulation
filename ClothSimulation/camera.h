#pragma once

#include <GL/glew.h> 
#include <algorithm>
#include "matrixmath.h"
#include "userinput.h"
#include "mat4.h"

using namespace std;

class Camera
{
public:
	Camera(float aspectRatio);
	void update(double delta_time);
	Mat4 getViewMatrix();
	Mat4 getProjectionMatrix();

private:
	void updateViewMatrix(); 
	double xPos;
	double yPos;
	double zPos;
	double distance;
	double orbitAngle;
	double tiltAngle;
	Mat4 viewMatrix;
	Mat4 projMatrix;

	const double MOUSE_ROTATION_SENSITIVITY = 0.015;
	const double DOLLY_SENSITIVITY = 0.1;
	const double MIN_DOLLY = 0.5;
	const double MAX_DOLLY = 10;
};