
#include "camera.h"

using namespace std;

// create a new instance of the camera class, this inizializes all 
// variables and creates the perspective projection matrix for the camera. 
Camera::Camera(float aspectRatio)
{
	xPos = 0.0f;	// the camera centeres this position in frame
	yPos = 2.0f;
	zPos = 0.0f;
	distance = 2.5f;	// the distance from the camera the the center position in OpenGL units. 
	orbitAngle = 0.0f;	// the camera's orbiting angle around the center position in radians
	tiltAngle = -0.5f;	// the camera's tilt angle around the center position in radians
	updateViewMatrix(); 
	MatrixMath::mat4proj(projMatrix, aspectRatio, 1.2f);
}

// Update the cameras position, call this function in the main update loop.
void Camera::update(double delta_time)
{
	// zoom with the mouse scroll wheel. 
	distance += -UserInput::getMouseDeltaScroll() * DOLLY_SENSITIVITY * distance;
	distance = max(MIN_DOLLY, min(distance, MAX_DOLLY));

	// rotate with the mouse. 
	if (UserInput::getLeftMouseButton())
	{
		double dx = 0, dy = 0;
		UserInput::getMouseVel(dx, dy);
		orbitAngle -= dx * MOUSE_ROTATION_SENSITIVITY;
		tiltAngle -= dy * MOUSE_ROTATION_SENSITIVITY;
		tiltAngle = max(-3.14 / 2, min(tiltAngle, 3.14 / 2));
	}
	
	updateViewMatrix(); 
}

void Camera::getViewMatrix(GLfloat matrix[])
{
	for (int i = 0; i < 16; i++) {
		matrix[i] = viewMatrix[i]; 
	}
}

void Camera::getProjectionMatrix(GLfloat matrix[])
{
	for (int i = 0; i < 16; i++) {
		matrix[i] = projMatrix[i];
	}
}


// This function also updates the camera's view matrix using 
// the variables describing it's position and rotation.
void Camera::updateViewMatrix()
{	
	GLfloat mainTranslation[16]; 	
	GLfloat orbitRotation[16];
	GLfloat tiltRotation[16];
	GLfloat distanceTranslation[16];

	MatrixMath::translateMat4(mainTranslation, (float)-xPos, (float)-yPos, (float)-zPos);
	MatrixMath::rotyMat4(orbitRotation, (float)orbitAngle);
	MatrixMath::rotxMat4(tiltRotation, (float)tiltAngle);
	MatrixMath::translateMat4(distanceTranslation, 0, 0, (float)-distance);

	GLfloat temp1[16];
	GLfloat temp2[16];	
	MatrixMath::multMat4(mainTranslation, orbitRotation, temp1);
	MatrixMath::multMat4(tiltRotation, distanceTranslation, temp2);
	MatrixMath::multMat4(temp1, temp2, viewMatrix);
}