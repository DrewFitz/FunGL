#ifndef __MATRIX_H__
#define __MATRIX_H__

#include <OpenGL/gl3.h>
#include <cmath>
#include <cstring>

namespace Matrix {

//typedef GLfloat Matrix[16];
struct Matrix {
	GLfloat m[16];
	GLfloat& operator[] (const int& index)
	{
		return m[index];
	}
	Matrix& operator= (const Matrix& other)
	{
		std::memcpy(&m, &other.m, sizeof(m));
		return *this;
	}
};
struct Vector {
	GLfloat w;
	GLfloat x;
	GLfloat y;
	GLfloat z;
	Vector(): w(1.0f), x(0.0f), y(0.0f), z(0.0f) {}
	Vector(GLfloat _x, GLfloat _y, GLfloat _z) : w(1.0f), x(_x), y(_y), z(_z) {}
	inline void normalize() {
		GLfloat mag = sqrtf(x * x + y * y + z * z);
		x /= mag;
		y /= mag;
		z /= mag;
		w = 1.0f;
	}
};

inline void makeIdentityMatrix(Matrix& out);
inline void makeTranslationMatrix(Matrix& out, Vector displacement);
inline void makeRotationMatrix(Matrix& out, const Vector& axis, float angle);
inline void makeScalingMatrix(Matrix& out, const Vector& scaleVec);
inline void makeUniformScalingMatrix(Matrix& out, float scale);
inline void makeProjectionMatrix(Matrix& out, float fov, float near, float far, float aspect);
inline void makeOrthonalMatrix(Matrix& out, float width, float height, float near, float far);
inline void matrixMultiply(Matrix& out, Matrix& l, Matrix& r);

inline void makeIdentityMatrix(Matrix& out)
{
	out[0] = 1.0;
	out[1] = 0.0;
	out[2] = 0.0;
	out[3] = 0.0;

	out[4] = 0.0;
	out[5] = 1.0;
	out[6] = 0.0;
	out[7] = 0.0;

	out[8] = 0.0;
	out[9] = 0.0;
	out[10] = 1.0;
	out[11] = 0.0;

	out[12] = 0.0;
	out[13] = 0.0;
	out[14] = 0.0;
	out[15] = 1.0;
}
inline void makeTranslationMatrix(Matrix& out, Vector displacement)
{
	makeIdentityMatrix(out);

	out[12] = displacement.x;
	out[13] = displacement.y;
	out[14] = displacement.z;
}
inline void makeRotationMatrix(Matrix& out, const Vector& axis, float angle)
{
	makeIdentityMatrix(out);

	GLfloat x = axis.x, y = axis.y, z = axis.z;

	out[0] = cosf(angle) + x * x *(1-cosf(angle));
	out[1] = y * x *(1-cosf(angle)) + z * sinf(angle);
	out[2] = z * x *(1-cosf(angle)) - y * sinf(angle);

	out[4] = x * y *(1-cosf(angle)) - z * sinf(angle);
	out[5] = cosf(angle) + y * y *(1-cosf(angle));
	out[6] = z * y *(1-cosf(angle)) + x * sinf(angle);

	out[8] = x * z *(1-cosf(angle)) + y * sinf(angle);
	out[9] = y * z *(1-cosf(angle)) - x * sinf(angle);
	out[10] = cosf(angle) + z * z *(1-cosf(angle));
}
inline void makeScalingMatrix(Matrix& out, const Vector& scaleVec)
{
	makeIdentityMatrix(out);

	out[0]  = scaleVec.x;
	out[5]  = scaleVec.y;
	out[10] = scaleVec.z;
}
inline void makeUniformScalingMatrix(Matrix& out, float scale)
{
	makeIdentityMatrix(out);

	out[0]  = scale;
	out[5]  = scale;
	out[10] = scale;
}
inline void makeProjectionMatrix(Matrix& out, float fov, float near, float far, float aspect)
{
	makeIdentityMatrix(out);
	float e = 1.0f / tanf(fov * (M_PI/360.0f));

	out[0]  = e / aspect;
	out[5]  = e;
	out[10] = (far+near) / (near-far);
	out[11] = -1;
	out[14] = 2.0f * far * near / (near-far);
	out[15] = 0;
}
inline void matrixMultiply(Matrix& out, Matrix& l, Matrix& r)
{
	Matrix temp;
	makeIdentityMatrix(temp);
	// OpenGL expects column-major matrices
	/* 0  4  8 12 *
	 * 1  5  9 13 *
	 * 2  6 10 14 *
	 * 3  7 11 15 */
	for (int i = 0; i < 16; ++i)
	{
		int row = i % 4;
		int col = (i / 4) * 4;
		temp[i]	= l[col]   * r[row]
				+ l[col+1] * r[row+4]
				+ l[col+2] * r[row+8]
				+ l[col+3] * r[row+12];
	}
	out = temp;
}

inline void makeOrthonalMatrix(Matrix& out, float width, float height, float near, float far)
{
	makeIdentityMatrix(out);
	
	out[0]  = 2.0f / width;
	out[5]  = 2.0f / height;
	out[10] = -2.0f/(far-near);
	out[14] = -(far+near)/(far-near);
}

inline void matrixMultiply(Vector& out, Matrix& m, Vector& v)
{
	Vector temp;
	temp.x = v.x * m[0] + v.x * m[1] + v.x * m[2] + v.x * m[3];
	temp.y = v.y * m[4] + v.y * m[5] + v.y * m[6] + v.y * m[7];
	temp.z = v.z * m[8] + v.z * m[9] + v.z * m[10] + v.z * m[11];
	temp.w = v.w * m[12] + v.w * m[13] + v.w * m[14] + v.w * m[15];
	out = temp;
}

} // Namespace Matrix



#endif /* __MATRIX_H__ */
