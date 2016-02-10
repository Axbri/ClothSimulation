#pragma once

#include "camera.h"
#include "vec4.h"
#include "vec3.h"
#include "vec2.h"

class MousePicker
{
public:

	MousePicker(Vec2 screenSize);

	void setScreenSize(Vec2 size);
	Vec3 calculateMouseRay(Camera camera); 
	Vec3 calculateStartPoint(Camera camera);
	Vec3 getPlaneIntersectionPoint(double planeHeight);
	Vec3 getCurrentRay(); 
	Vec3 getCurrentStartPoint(); 
	bool isPlaneIntersectionValid(); 


private: 
	int PLANE_SEARCH_DISTANCE = 50;
	int PLANE_SEARCH_ITERATIONS = 100; 
	Vec2 screenSize;
	bool planeIntersectionValid; 
	Vec3 currentMouseRay; 
	Vec3 currentStartPoint; 

};

