#pragma once

#include "camera.h"
#include "vec4.h"
#include "vec3.h"
#include "vec2.h"

class MousePicker
{
public:

	MousePicker(Vec2 screenSize);

	Vec3 calculateMouseRay(Camera camera); 
	Vec3 getRayStartPoint(Camera camera); 
	void setScreenSize(Vec2 size);

private: 
	Vec2 screenSize;

};

