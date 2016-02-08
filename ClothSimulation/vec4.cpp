#include "vec4.h"



Vec4::Vec4()
{
	x1 = 0;
	x2 = 0;
	x3 = 0;
	x4 = 0;
}

Vec4::Vec4(double x1Value, double x2Value, double x3Value, double x4Value)
{
	x1 = x1Value;
	x2 = x2Value;
	x3 = x3Value;
	x4 = x4Value;
}

Vec4::Vec4(const Vec4 & other)
{
	x1 = other.x1;
	x2 = other.x2;
	x3 = other.x3;
	x4 = other.x4;
}

void Vec4::operator=(const Vec4 &v)
{
	x1 = v.x1;
	x2 = v.x2;
	x3 = v.x3;
	x4 = v.x4;
}

Vec4 Vec4::operator+(const Vec4 &v) const
{
	return Vec4(x1 + v.x1, x2 + v.x2, x3 + v.x3, x4 + v.x4);
}

Vec4 Vec4::operator-(const Vec4 &v) const
{
	return Vec4(x1 - v.x1, x2 - v.x2, x3 - v.x3, x4 - v.x4);
}

void Vec4::operator+=(const Vec4 &v)
{
	x1 += v.x1;
	x2 += v.x2;
	x3 += v.x3;
	x4 += v.x4;
}
void Vec4::operator-=(const Vec4 &v)
{
	x1 -= v.x1;
	x2 -= v.x2;
	x3 -= v.x3;
	x4 -= v.x4;
}

float Vec4::operator*(const Vec4 &v)
{
	return x1 * v.x1 + x2 * v.x2 + x3 * v.x3 + x4 * v.x4;
}

Vec4 Vec4::operator*(const float &c) const
{
	return Vec4(x1*c, x2*c, x3*c, x4*c);
}

void Vec4::normalize()
{
	float length = this->length();
	x1 *= 1.0 / length;
	x2 *= 1.0 / length;
	x3 *= 1.0 / length;
	x4 *= 1.0 / length;
}

double Vec4::length()
{
	return sqrt(x1 * x1 + x2 * x2 + x3 * x3 + x4 * x4);
}

