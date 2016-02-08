#pragma once

#include "camera.h"
#include "vec4.h"
#include "vec3.h"
#include "vec2.h"

class MousePicker
{
public:

	static Vec3 calculateMouseRay(const Camera camera); 

};

