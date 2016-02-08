#include "mousepicker.h"



MousePicker::MousePicker(Vec2 screenSize)
{
	this->screenSize = Vec2{ screenSize }; 
}

Vec3 MousePicker::calculateMouseRay(Camera camera)
{
	Vec2 normalizedDeviceCoords{ UserInput::getMouseNormalizedDeviceCoords(screenSize.x, screenSize.y) };

	Vec4 clipSpaceCoords{ normalizedDeviceCoords.x, normalizedDeviceCoords.y, -1.0f, 1.0f };

	Mat4 inverseProjectionMatrix{ camera.getProjectionMatrix() };
	inverseProjectionMatrix = inverseProjectionMatrix.inverse(); 

	Vec4 eyeSpaceCoords = inverseProjectionMatrix * clipSpaceCoords; 

	Mat4 inverseVievMatrix{ camera.getViewMatrix() };
	inverseVievMatrix = inverseVievMatrix.inverse(); 

	Vec4 worldSpaceRay = inverseVievMatrix * eyeSpaceCoords; 
	Vec3 mouseRay{ worldSpaceRay.x1, worldSpaceRay.x2, worldSpaceRay.x3 }; 

	mouseRay.normalize(); 

	return mouseRay;
}

Vec3 MousePicker::getRayStartPoint(Camera camera)
{
	Vec3 cameraPos{ camera.getPosition() }; 
	Vec3 viewDirection{ camera.getViewVector() }; 
	
	viewDirection = viewDirection * camera.getDistance();

	cameraPos -= viewDirection; 

	return Vec3{ cameraPos };
}

void MousePicker::setScreenSize(Vec2 size)
{
	screenSize = Vec2{ size };
}


