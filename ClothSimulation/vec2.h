#pragma once

#include <math.h>

class Vec2
{
public:
	Vec2();
	Vec2(double xValue, double yValue);
	Vec2(const Vec2 &other);

	void normalize();
	double length();
	void set(Vec2 other);
	void set(double xValue, double yValue);

	double x, y;
};

