#ifndef MATRIXMATH_H_INCLUDED
#define MATRIXMATH_H_INCLUDED

#include <GL/glew.h> 
#include <math.h>

using namespace std;

class MatrixMath
{
public:
	// represents a 4x4 matrix using a array of floats.
	// Data are stored like this:
	// [ 1] [ 2] [ 3] [ 4]
	// [ 5] [ 6] [ 7] [ 8]
	// [ 9] [10] [11] [12]
	// [13] [14] [15] [16]

	static void multMat4(float M1[], float M2[], float Mout[]);
	static void identityMat4(float M[]);
	static void translateMat4(float M[], float dx, float dy, float dz);
	static void scaleMat4(float M[], float sx, float sy, float sz);
	static void rotxMat4(float M[], float angle);
	static void rotyMat4(float M[], float angle);
	static void rotzMat4(float M[], float angle);
	static void mat4proj(float M[], float aspectRatio, float fov);	
};

#endif
