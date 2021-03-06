#include "mousepicker.h"



MousePicker::MousePicker()
{ 
	this->planeIntersectionValid = false;
}

// Calculate the vector from the camera into the world trough the mouse cursor. 
// The returned vector is in world coordinates, and it is normalized. 
Vec3 MousePicker::calculateMouseRay(Camera camera)
{
	Vec2 normalizedDeviceCoords{ UserInput::getMouseNormalizedDeviceCoords() };
	Vec4 clipSpaceCoords{ normalizedDeviceCoords.x, normalizedDeviceCoords.y, -1.0f, 1.0f };

	Mat4 inverseProjectionMatrix{ camera.getProjectionMatrix().inverse() };
	Vec4 temp{ inverseProjectionMatrix * clipSpaceCoords };
	Vec4 eyeSpaceCoords{ temp.x1, temp.x2, -1.0, 0.0 }; 

	Mat4 inverseVievMatrix{ camera.getViewMatrix().inverse() };
	Vec4 worldSpaceRay{ inverseVievMatrix * eyeSpaceCoords };

	Vec3 mouseRay{ worldSpaceRay.x1, worldSpaceRay.x2, worldSpaceRay.x3 }; 		
	mouseRay.normalize();
	currentMouseRay = Vec3{ mouseRay };
	return currentMouseRay;
}

// calculate the world position of the camera. 
Vec3 MousePicker::calculateStartPoint(Camera camera)
{
	Vec3 cameraPos{ camera.getPosition() }; 
	Vec3 viewDirection{ camera.getViewVector() }; 	
	viewDirection = viewDirection * (camera.getDistance() + 1);	// I dont understand why I need +1 here...
	currentStartPoint = cameraPos - viewDirection;
	return currentStartPoint;
}

// calculate the world coordinates of the intrecestion point between the 
// mouse ray the the plane: y = planeHeight. This is done using "binary search". 
// if no valid intersection point is found, the vector (0, 0, 0) is returned.
// Call the function: isPlaneIntersectionValid() to check if it is valid. 
Vec3 MousePicker::getPlaneIntersectionPoint(double planeHeight)
{
	planeIntersectionValid = false; 
	Vec3 intesectionPoint = currentStartPoint;
	double distance = PLANE_SEARCH_DISTANCE / 2.0;
	Vec3 deltaVec = currentMouseRay * distance; 
	intesectionPoint += deltaVec; 
	
	for (int i = 0; i < PLANE_SEARCH_ITERATIONS; i++)
	{
		distance = distance / 2.0;
		if (intesectionPoint.y > planeHeight)
		{
			deltaVec = currentMouseRay * distance; 
		}
		else
		{
			deltaVec = currentMouseRay * (-distance);
			planeIntersectionValid = true; 
		}
		intesectionPoint += deltaVec; 
	}

	if (planeIntersectionValid)
	{
		return Vec3{ intesectionPoint };
	}
	else
	{
		return Vec3{ 0, 0, 0 };
	}
}

Vec3 MousePicker::getCurrentRay()
{
	return Vec3{ currentMouseRay };
}

Vec3 MousePicker::getCurrentStartPoint()
{
	return Vec3{ currentStartPoint };
}

bool MousePicker::isPlaneIntersectionValid()
{
	return planeIntersectionValid;
}




