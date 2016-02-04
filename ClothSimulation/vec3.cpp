#include "vec3.h"

Vec3::Vec3()
{
	x = 0;
	y = 0;
	z = 0;
}

Vec3::Vec3(double xValue, double yValue, double zValue)
{
	x = xValue;
	y = yValue;
	z = zValue;
}

Vec3::Vec3(const Vec3 &other)
{
	x = other.x;
	y = other.y;
	z = other.z;
}

void Vec3::normalize()
{
	float length = this->length();
	x *= 1.0 / length;
	y *= 1.0 / length;
	z *= 1.0 / length;
}

double Vec3::length() 
{
	return sqrt(x * x + y * y + z * z);
}

void Vec3::cross(Vec3 v1, Vec3 v2)
{
	x = v1.y * v2.z - v1.z * v2.y; 
	y = v1.z * v2.x - v1.x * v2.z;
	z = v1.x * v2.y - v1.y * v2.x;
}

void Vec3::subtract(Vec3 other)
{
	x -= other.x; 
	y -= other.y; 
	z -= other.z; 
}

void Vec3::set(Vec3 other)
{
	x = other.x;
	y = other.y;
	z = other.z;
}

void Vec3::set(double xValue, double yValue, double zValue)
{
	x = xValue;
	y = yValue;
	z = zValue;
}
