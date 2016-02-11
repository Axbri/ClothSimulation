#include "mat4.h"

Mat4::Mat4()
{
	loadIdentity(); 
}

Mat4::Mat4(double data[])
{
	for (int i = 0; i < 16; i++) {
		M[i] = data[i];
	}
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

void Mat4::loadPerspectiveProjection(double aspectRatio, double fov, double near, double far)
{
	this->loadIdentity();
	double horizontalFov = near * (float)sin(fov);
	double verticalFov = horizontalFov * aspectRatio;
	double left = -horizontalFov;
	double right = horizontalFov;
	double bottom = -verticalFov;
	double top = verticalFov;

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
	// http://stackoverflow.com/questions/1148309/inverting-a-4x4-matrix

	double inv[16];
	double invOut[16];
	float det;
	int i;

	inv[0] = M[5] * M[10] * M[15] -
		M[5] * M[11] * M[14] -
		M[9] * M[6] * M[15] +
		M[9] * M[7] * M[14] +
		M[13] * M[6] * M[11] -
		M[13] * M[7] * M[10];

	inv[4] = -M[4] * M[10] * M[15] +
		M[4] * M[11] * M[14] +
		M[8] * M[6] * M[15] -
		M[8] * M[7] * M[14] -
		M[12] * M[6] * M[11] +
		M[12] * M[7] * M[10];

	inv[8] = M[4] * M[9] * M[15] -
		M[4] * M[11] * M[13] -
		M[8] * M[5] * M[15] +
		M[8] * M[7] * M[13] +
		M[12] * M[5] * M[11] -
		M[12] * M[7] * M[9];

	inv[12] = -M[4] * M[9] * M[14] +
		M[4] * M[10] * M[13] +
		M[8] * M[5] * M[14] -
		M[8] * M[6] * M[13] -
		M[12] * M[5] * M[10] +
		M[12] * M[6] * M[9];

	inv[1] = -M[1] * M[10] * M[15] +
		M[1] * M[11] * M[14] +
		M[9] * M[2] * M[15] -
		M[9] * M[3] * M[14] -
		M[13] * M[2] * M[11] +
		M[13] * M[3] * M[10];

	inv[5] = M[0] * M[10] * M[15] -
		M[0] * M[11] * M[14] -
		M[8] * M[2] * M[15] +
		M[8] * M[3] * M[14] +
		M[12] * M[2] * M[11] -
		M[12] * M[3] * M[10];

	inv[9] = -M[0] * M[9] * M[15] +
		M[0] * M[11] * M[13] +
		M[8] * M[1] * M[15] -
		M[8] * M[3] * M[13] -
		M[12] * M[1] * M[11] +
		M[12] * M[3] * M[9];

	inv[13] = M[0] * M[9] * M[14] -
		M[0] * M[10] * M[13] -
		M[8] * M[1] * M[14] +
		M[8] * M[2] * M[13] +
		M[12] * M[1] * M[10] -
		M[12] * M[2] * M[9];

	inv[2] = M[1] * M[6] * M[15] -
		M[1] * M[7] * M[14] -
		M[5] * M[2] * M[15] +
		M[5] * M[3] * M[14] +
		M[13] * M[2] * M[7] -
		M[13] * M[3] * M[6];

	inv[6] = -M[0] * M[6] * M[15] +
		M[0] * M[7] * M[14] +
		M[4] * M[2] * M[15] -
		M[4] * M[3] * M[14] -
		M[12] * M[2] * M[7] +
		M[12] * M[3] * M[6];

	inv[10] = M[0] * M[5] * M[15] -
		M[0] * M[7] * M[13] -
		M[4] * M[1] * M[15] +
		M[4] * M[3] * M[13] +
		M[12] * M[1] * M[7] -
		M[12] * M[3] * M[5];

	inv[14] = -M[0] * M[5] * M[14] +
		M[0] * M[6] * M[13] +
		M[4] * M[1] * M[14] -
		M[4] * M[2] * M[13] -
		M[12] * M[1] * M[6] +
		M[12] * M[2] * M[5];

	inv[3] = -M[1] * M[6] * M[11] +
		M[1] * M[7] * M[10] +
		M[5] * M[2] * M[11] -
		M[5] * M[3] * M[10] -
		M[9] * M[2] * M[7] +
		M[9] * M[3] * M[6];

	inv[7] = M[0] * M[6] * M[11] -
		M[0] * M[7] * M[10] -
		M[4] * M[2] * M[11] +
		M[4] * M[3] * M[10] +
		M[8] * M[2] * M[7] -
		M[8] * M[3] * M[6];

	inv[11] = -M[0] * M[5] * M[11] +
		M[0] * M[7] * M[9] +
		M[4] * M[1] * M[11] -
		M[4] * M[3] * M[9] -
		M[8] * M[1] * M[7] +
		M[8] * M[3] * M[5];

	inv[15] = M[0] * M[5] * M[10] -
		M[0] * M[6] * M[9] -
		M[4] * M[1] * M[10] +
		M[4] * M[2] * M[9] +
		M[8] * M[1] * M[6] -
		M[8] * M[2] * M[5];

	det = M[0] * inv[0] + M[1] * inv[4] + M[2] * inv[8] + M[3] * inv[12];

	if (det != 0) {
		det = 1.0f / det;
		for (i = 0; i < 16; i++)
		{
			invOut[i] = inv[i] * det;
		}			
	}

	return Mat4(invOut);
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

Vec4 Mat4::operator*(const Vec4 & vec) const
{
	double x1 = M[0] * vec.x1 + M[1] * vec.x2 + M[2] * vec.x3 + M[3] * vec.x4;
	double x2 = M[4] * vec.x1 + M[5] * vec.x2 + M[6] * vec.x3 + M[7] * vec.x4;
	double x3 = M[8] * vec.x1 + M[9] * vec.x2 + M[10] * vec.x3 + M[11] * vec.x4;
	double x4 = M[12] * vec.x1 + M[13] * vec.x2 + M[14] * vec.x3 + M[15] * vec.x4;

	return Vec4(x1, x2, x3, x4);
}

