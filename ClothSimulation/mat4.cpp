#include "mat4.h"

Mat4::Mat4()
{
	loadIdentity(); 
}

Mat4::Mat4(const Mat4 &m)
{
	for (int i = 0; i < 16; i++) {
		M[i] = m.M[i];
	}
}

void Mat4::loadIdentity()
{
	for (int i = 0; i < 16; i++) {
		if (i == 0 || i == 5 || i == 10 || i == 15)
		{
			M[i] = 1;
		}
		else
		{
			M[i] = 0;
		}
	}
}

void Mat4::loadRotationX(double angle)
{
	this->loadIdentity();
	M[5] = cos(angle);
	M[6] = sin(angle);
	M[9] = -sin(angle);
	M[10] = cos(angle);
}

void Mat4::loadRotationY(double angle)
{
	this->loadIdentity();
	M[0] = cos(angle);
	M[2] = -sin(angle);
	M[8] = sin(angle);
	M[10] = cos(angle);
}

void Mat4::loadRotationZ(double angle)
{
	this->loadIdentity();
	M[0] = cos(angle);
	M[1] = sin(angle);
	M[4] = -sin(angle);
	M[5] = cos(angle);
}

void Mat4::loadScale(double sx, double sy, double sz)
{
	this->loadIdentity();
	M[0] = sx;
	M[5] = sy;
	M[10] = sz;
}

void Mat4::loadTranslation(double dx, double dy, double dz)
{
	this->loadIdentity();
	M[3] = dx;
	M[7] = dy;
	M[11] = dz;
}

void Mat4::loadPerspectiveProjection(float aspectRatio, float fov)
{
	this->loadIdentity();
	float near = 0.01;
	float far = 100;
	float horizontalFov = near * (float)sin(fov);
	float verticalFov = horizontalFov * aspectRatio;
	float left = -horizontalFov;
	float right = horizontalFov;
	float bottom = -verticalFov;
	float top = verticalFov;

	M[0] = (2 * near) / (right - left);
	M[2] = (right + left) / (right - left);
	M[5] = (2 * near) / (top - bottom);
	M[6] = (top + bottom) / (top - bottom);
	M[10] = -(far + near) / (far - near);
	M[11] = -(2 * far * near) / (far - near);
	M[14] = -1.0f;
}

Mat4 Mat4::inverse() const
{

	// TODO...

	return Mat4();
}

void Mat4::operator=(const Mat4 & m)
{
	for (int i = 0; i < 16; i++) {
		M[i] = m.M[i];
	}
}

Mat4 Mat4::operator*(const Mat4 & m) const
{
	Mat4 result; 
	int i, j, k;
	for (i = 0; i < 4; i++)
	{
		for (j = 0; j < 4; j++)
		{
			float sum = 0.0;
			for (k = 0; k < 4; k++)
			{
				sum = sum + M[(4 * k) + j] * m.M[(4 * i) + k];
			}
			result.M[(4 * i) + j] = sum;
		}
	}

	return result;
}

Vec4 Mat4::operator*(const Vec4 & m) const
{

	// TODO...

	return Vec4();
}

