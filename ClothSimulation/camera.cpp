
#include "camera.h"

using namespace std;

// create a new instance of the camera class, this inizializes all 
// variables and creates the perspective projection matrix for the camera. 
Camera::Camera()
{
	position = Vec3{ 0.0f, 0.8f, 0.0f };
	distance = (MIN_DOLLY + MAX_DOLLY) / 2.0;	// the distance from the camera the the center position in OpenGL units. 
	orbitAngle = 1.2;	// the camera's orbiting angle around the center position in radians
	tiltAngle = 0.5;	// the camera's tilt angle around the center position in radians

	targetDistance = 0; 
	targetOrbitAngle = 0;
	targetTiltAngle = 0; 

	updateViewMatrix(); 
	double aspectRatio = UserInput::getWindowSize().y / UserInput::getWindowSize().x;
	projMatrix.loadPerspectiveProjection(aspectRatio, 1.2f, NEAR_CLIP, FAR_CLIP);
}

// Update the cameras position, call this function in the main update loop.
void Camera::update(double delta_time)
{
	// use the aspect ratio of the window to update the projekction matrix. 
	double aspectRatio = UserInput::getWindowSize().y / UserInput::getWindowSize().x;
	projMatrix.loadPerspectiveProjection(aspectRatio, 1.2f, NEAR_CLIP, FAR_CLIP);

	// user input: zoom with the mouse scroll wheel. 
	targetDistance += -UserInput::getMouseDeltaScroll() * DOLLY_SENSITIVITY * targetDistance;
	targetDistance = max(MIN_DOLLY, min(targetDistance, MAX_DOLLY));

	// user input: rotate with the mouse. 
	if (UserInput::getCenterMouseButton())
	{
		Vec2 mouseVel = UserInput::getMouseVel();
		targetOrbitAngle -= mouseVel.x * MOUSE_ROTATION_SENSITIVITY;
		targetTiltAngle -= mouseVel.y * MOUSE_ROTATION_SENSITIVITY;
		targetTiltAngle = max(-3.14 / 2, min(targetTiltAngle, 0.0));
	}

	// smoothly move the camera around according to the target values. 
	distance += (targetDistance - distance) * SMOOTH_DOLLY * delta_time;
	orbitAngle += (targetOrbitAngle - orbitAngle) * SMOOTH_MOUSE_MOVMENT * delta_time;
	tiltAngle += (targetTiltAngle - tiltAngle) * SMOOTH_MOUSE_MOVMENT * delta_time;
	
	// update the view matrix with the new position of the camera. 
	updateViewMatrix(); 
}

Mat4 Camera::getViewMatrix()
{
	return viewMatrix; 
}

Mat4 Camera::getProjectionMatrix()
{
	return projMatrix; 
}

Vec3 Camera::getPosition()
{
	return Vec3{ position };
}

Vec3 Camera::getViewVector()
{
	return Vec3{ -sin(orbitAngle) * cos(tiltAngle), sin(tiltAngle), -cos(orbitAngle) * cos(tiltAngle) };
}

double Camera::getDistance() const
{
	return distance;
}

// This function also updates the camera's view matrix using 
// the variables describing it's position and rotation.
void Camera::updateViewMatrix()
{	
	Mat4 mainTranslation, orbitRotation, tiltRotation, distanceTranslation;

	mainTranslation.loadTranslation((float)-position.x, (float)-position.y, (float)-position.z);
	orbitRotation.loadRotationY((float)orbitAngle); 
	tiltRotation.loadRotationX((float)tiltAngle);
	distanceTranslation.loadTranslation(0, 0, (float)-distance); 

	viewMatrix = mainTranslation * orbitRotation * tiltRotation * distanceTranslation; 
}