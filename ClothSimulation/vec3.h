#pragma once

#include <math.h>

class Vec3
{
public:
	Vec3();
	Vec3(double xValue, double yValue, double zValue);
	Vec3(const Vec3 &other);

	void normalize();
	double length(); 
	void cross(Vec3 v1, Vec3 v2);
	void subtract(Vec3 other);
	void set(Vec3 other);
	void set(double xValue, double yValue, double zValue);

	double x, y, z; 
};

