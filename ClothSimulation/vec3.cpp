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

void Vec3::operator=(const Vec3 &v)
{
	x = v.x;
	y = v.y;
	z = v.z;
}

Vec3 Vec3::operator+(const Vec3 &v) const
{
	return Vec3(x + v.x, y + v.y, z + v.z);
}

Vec3 Vec3::operator-(const Vec3 &v) const
{
	return Vec3(x - v.x, y - v.y, z - v.z);
}

void Vec3::operator+=(const Vec3 &v) 
{
	x += v.x;
	y += v.y;
	z += v.z;
}
void Vec3::operator-=(const Vec3 &v)
{
	x -= v.x;
	y -= v.y;
	z -= v.z;
}

double Vec3::operator*(const Vec3 &v)
{
	return x*v.x + y*v.y + z*v.z;
}

Vec3 Vec3::operator*(const float &c) const
{
	return Vec3(x*c, y*c, z*c);
}

void Vec3::normalize()
{
	double length = this->length();
	x *= 1.0 / length;
	y *= 1.0 / length;
	z *= 1.0 / length;
}

double Vec3::length() 
{
	double lengthSquared = x * x + y * y + z * z;
	return lengthSquared * invSqrt(lengthSquared);
} 

double Vec3::lengthSquared()
{
	return x * x + y * y + z * z;
}

float Vec3::invSqrt(float number)
{
	// Magical Square Root Implementation from Quake III
	long i;
	float x2, y;
	const float threehalfs = 1.5F;

	x2 = number * 0.5F;
	y = number;
	i = *(long *)&y;  // evil floating point bit level hacking
	i = 0x5f3759df - (i >> 1); // what the fuck?
	y = *(float *)&i;
	y = y * (threehalfs - (x2 * y * y)); // 1st iteration
	// y  = y * ( threehalfs - ( x2 * y * y ) ); // 2nd iteration, this can be removed

	return y;
}

float Vec3::squareRoot(float x)	// some other function, does not seam to be more efisient than sqrt(); 
{
	unsigned int i = *(unsigned int*)&x;

	// adjust bias
	i += 127 << 23;
	// approximation of square root
	i >>= 1;

	return *(float*)&i;
}

void Vec3::cross(Vec3 v1, Vec3 v2)
{
	x = v1.y * v2.z - v1.z * v2.y; 
	y = v1.z * v2.x - v1.x * v2.z;
	z = v1.x * v2.y - v1.y * v2.x;
}

double Vec3::pointLineDistance(Vec3 point, Vec3 line)
{
	Vec3 uParalellN = line * ((point * line) / line.lengthSquared());
	Vec3 uOrtogonalN = point - uParalellN; 
	return uOrtogonalN.length();
}

double Vec3::toPointDistanceParallelToLine(Vec3 point, Vec3 line)
{
	Vec3 uParalellN = line * ((point * line) / line.lengthSquared());
	return uParalellN.length();
}
