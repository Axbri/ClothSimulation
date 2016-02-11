#pragma once

#include <GL/glew.h> 
#include <math.h>
#include "vec4.h"

class Mat4
{
public:

	Mat4();
	Mat4(double data[]);
	Mat4(const Mat4 &m);

	void loadIdentity(); 
	void loadRotationX(double angle); 
	void loadRotationY(double angle);
	void loadRotationZ(double angle);
	void loadScale(double sx, double sy, double sz); 
	void loadTranslation(double dx, double dy, double dz);
	void loadPerspectiveProjection(double aspectRatio, double fov, double near, double far);

	Mat4 inverse() const; 

	void operator=(const Mat4 &m);
	Mat4 operator*(const Mat4 &m) const;
	Vec4 operator*(const Vec4 &vec) const;

	GLfloat M[16];
	// represents a 4x4 matrix using a array of floats.
	// Data are stored like this:
	// [ 1] [ 2] [ 3] [ 4]
	// [ 5] [ 6] [ 7] [ 8]
	// [ 9] [10] [11] [12]
	// [13] [14] [15] [16]
};

