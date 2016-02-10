#pragma once

#include <math.h>

class Vec3
{
public:
	Vec3();
	Vec3(double xValue, double yValue, double zValue);
	Vec3(const Vec3 &other);

	void operator=(const Vec3 &v); 
	Vec3 operator+(const Vec3 &v) const;
	Vec3 operator-(const Vec3 &v) const; 
	void operator+=(const Vec3 &v);
	void operator-=(const Vec3 &v);
	float operator*(const Vec3 &v);
	Vec3 operator*(const float &c) const;

	void normalize();
	double length(); 
	double lengthSquared();
	void cross(Vec3 v1, Vec3 v2);
	static double pointLineDistance(Vec3 point, Vec3 line);
	static double toPointDistanceParallelToLine(Vec3 point, Vec3 line);
	double x, y, z; 
};

