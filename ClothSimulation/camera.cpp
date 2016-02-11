
#include "camera.h"

using namespace std;

// create a new instance of the camera class, this inizializes all 
// variables and creates the perspective projection matrix for the camera. 
Camera::Camera(double aspectRatio)
{
	position = Vec3{ 0.0f, 1.2f, 0.0f };
	distance = 2.5f;	// the distance from the camera the the center position in OpenGL units. 
	orbitAngle = 1.2f;	// the camera's orbiting angle around the center position in radians
	tiltAngle = -0.5f;	// the camera's tilt angle around the center position in radians
	updateViewMatrix(); 
	projMatrix.loadPerspectiveProjection(aspectRatio, 1.2f, NEAR_CLIP, FAR_CLIP);
}

// Update the cameras position, call this function in the main update loop.
void Camera::update(double delta_time)
{
	// zoom with the mouse scroll wheel. 
	distance += -UserInput::getMouseDeltaScroll() * DOLLY_SENSITIVITY * distance;
	distance = max(MIN_DOLLY, min(distance, MAX_DOLLY));

	// rotate with the mouse. 
	if (UserInput::getRightMouseButton())
	{
		Vec2 mouseVel = UserInput::getMouseVel();
		orbitAngle -= mouseVel.x * MOUSE_ROTATION_SENSITIVITY;
		tiltAngle -= mouseVel.y * MOUSE_ROTATION_SENSITIVITY;
		tiltAngle = max(-3.14 / 2, min(tiltAngle, 3.14 / 2));
	}
	
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