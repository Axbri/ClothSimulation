
#include "matrixmath.h"

using namespace std;

// Calculate the matrix multimlication of the two matrices M1 and M2. 
// The result is stored in matrix Mout. 
void MatrixMath::multMat4(float M1[], float M2[], float Mout[])
{
	int i, j, k;
	for (i = 0; i < 4; i++) 
	{
		for (j = 0; j < 4; j++) 
		{
			float sum = 0.0;
			for (k = 0; k < 4; k++)
			{
				sum = sum + M1[(4 * k) + j] * M2[(4 * i) + k];
			}				
			Mout[(4 * i) + j] = sum;
		}
	}
}

// Load the identity matrix (enhetsmatrisen) in the provided matrix M. 
void MatrixMath::identityMat4(float M[])
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

// Load a translation matrix in the provided matrix M. 
// dx, dy, and dz describes the translation in the x, y, and z dimensions. 
void MatrixMath::translateMat4(float M[], float dx, float dy, float dz)
{
	identityMat4(M);
	M[3] = dx;
	M[7] = dy;
	M[11] = dz;
}

// Load a scaling matrix in the provided matrix M. 
// sx, sy, and sz describes the scaling factor in the x, y, and z dimensions. 
void MatrixMath::scaleMat4(float M[], float sx, float sy, float sz)
{
	identityMat4(M);
	M[0] = sx;
	M[5] = sy;
	M[10] = sz;
}

// Load a counterclockwise (moturs) rotation matrix around 
// the X axis into the provided matrix M.
void MatrixMath::rotxMat4(float M[], float angle)
{
	identityMat4(M);
	M[5] = cos(angle);
	M[6] = sin(angle);
	M[9] = -sin(angle);
	M[10] = cos(angle);
}

// Load a counterclockwise (moturs) rotation matrix around 
// the Y axis into the provided matrix M.
void MatrixMath::rotyMat4(float M[], float angle)
{
	identityMat4(M);
	M[0] = cos(angle);
	M[2] = -sin(angle);
	M[8] = sin(angle);
	M[10] = cos(angle);
}

// Load a counterclockwise (moturs) rotation matrix around 
// the Z axis into the provided matrix M.
void MatrixMath::rotzMat4(float M[], float angle)
{
	identityMat4(M);
	M[0] = cos(angle);
	M[1] = sin(angle);
	M[4] = -sin(angle);
	M[5] = cos(angle);
}

// Load a parallel projection matrix into the provided matrix M. 
// aspectRatio is defined as window height devided by window width. 
// fov is the field of view used, unly FOVs up to 90 degrees are suported. 
void MatrixMath::mat4proj(float M[], float aspectRatio, float fov)
{
	identityMat4(M);
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