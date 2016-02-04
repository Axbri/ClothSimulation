#include "vec2.h"

Vec2::Vec2()
{
	x = 0;
	y = 0;
}

Vec2::Vec2(double xValue, double yValue)
{
	x = xValue;
	y = yValue;
}

Vec2::Vec2(const Vec2 & other)
{
	x = other.x;
	y = other.y;
}

void Vec2::normalize()
{
	float length = this->length();
	x *= 1.0 / length;
	y *= 1.0 / length;
}

double Vec2::length()
{
	return sqrt(x * x + y * y);
}

void Vec2::set(Vec2 other)
{
	x = other.x;
	y = other.y;
}

void Vec2::set(double xValue, double yValue)
{
	x = xValue;
	y = yValue;
}

