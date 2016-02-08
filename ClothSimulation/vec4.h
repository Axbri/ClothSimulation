#pragma once

#include <math.h>

class Vec4
{
public:
	Vec4();
	Vec4(double x1Value, double x2Value, double x3Value, double x4Value);
	Vec4(const Vec4 &other);
	
	void operator=(const Vec4 &v);
	Vec4 operator+(const Vec4 &v) const;
	Vec4 operator-(const Vec4 &v) const;
	void operator+=(const Vec4 &v);
	void operator-=(const Vec4 &v);
	float operator*(const Vec4 &v);
	Vec4 operator*(const float &c) const;

	void normalize();
	double length();

	double x1, x2, x3, x4;
};

