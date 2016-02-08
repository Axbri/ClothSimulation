#include "mousepicker.h"



Vec3 MousePicker::calculateMouseRay(const Camera camera)
{
	Vec2 normalizedDeviceCoords{ UserInput::getMousePos() };

	Vec4 clipSpaceCoords = { normalizedDeviceCoords.x, normalizedDeviceCoords.y, -1.0f, 1.0f };

	return Vec3();
}
