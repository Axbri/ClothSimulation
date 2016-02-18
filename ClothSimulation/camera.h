#pragma once

#include <GL/glew.h> 
#include <algorithm>
#include "userinput.h"
#include "mat4.h"

using namespace std;

class Camera
{
public:
	Camera();
	void update(double delta_time);
	Mat4 getViewMatrix();
	Mat4 getProjectionMatrix();
	Vec3 getPosition(); 
	Vec3 getViewVector(); 
	double getDistance() const;

	const double NEAR_CLIP = 0.01;
	const double FAR_CLIP = 100;

private:
	void updateViewMatrix(); 
	Vec3 position;
	double distance;
	double orbitAngle;
	double tiltAngle;
	double targetDistance;
	double targetOrbitAngle;
	double targetTiltAngle;
	Mat4 viewMatrix;
	Mat4 projMatrix;

	const double MOUSE_ROTATION_SENSITIVITY = 0.015;
	const double DOLLY_SENSITIVITY = 0.1;
	const double MIN_DOLLY = 1.5;
	const double MAX_DOLLY = 10;	

	const double SMOOTH_DOLLY = 7;
	const double SMOOTH_MOUSE_MOVMENT = 10;

};