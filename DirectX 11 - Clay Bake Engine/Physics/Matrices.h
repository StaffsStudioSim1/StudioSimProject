#pragma once
#ifndef _H_MATH_MARTICES_
#define _H_MATH_MARTICES_

#ifndef NO_EXTRAS
#include <ostream>
#endif // !NO_EXTRAS

#include "Vectors.h"

/*
Left Handed  matrices setup  
*/
// definition for a 2by2 matrix
typedef struct matrix2x2
{
	union 
	{
		struct
		{
			float _11, _12, 
				  _21, _22;
		};
		float asArray[4];
	};
	inline matrix2x2()
	{
		_11 = _22 = 1.0f;
		_12 = _21 = 0.0f;
	}
	inline matrix2x2(float f11, float f12,
					 float f21, float f22)
	{
		_11 = f11; _12 = f12;
		_21 = f21; _22 = f22;
	}
	inline float* operator[](int i)
	{
		return &(asArray[i * 2]);
	}

}matrix2x2; 
// definition for a 3by3 matrix
typedef struct matrix3x3
{
	union
	{
		struct
		{
			float _11, _12, _13,
			 	  _21, _22, _23,
				  _31, _32, _33;
		};
		float asArray[9];
	};
	inline matrix3x3()
	{
		_11 = _22 = _33 = 1.0f;
		_12 = _13 = _21 = 0.0f;
		_23 = _31 = _32 = 0.0f;
	}
	inline matrix3x3(float f11, float f12, float f13,
					 float f21, float f22, float f23,
					 float f31, float f32, float f33)
	{
		_11 = f11; _12 = f12; _13 = f13;
		_21 = f21; _22 = f22; _23 = f23;
		_31 = f31; _32 = f32; _33 = f33;
	}
	inline float* operator[] (int i)
	{
		return &(asArray[i * 3]);
	}
}matrix3x3;
// definition for a 4by4 matrix
typedef struct matrix4x4
{
	union
	{
		struct
		{
			float _11, _12, _13, _14,
				  _21, _22, _23, _24, 
				  _31, _32, _33, _34,
				  _41, _42, _43, _44;
		};
		float asArray[16];
	};
	inline matrix4x4()
	{
		_11 = _22 = _33 = _44 = 1.0f;
		_12 = _13 = _14 = _21 = 0.0f;
		_23 = _24 = _31 = _32 = 0.0f;
		_34 = _41 = _42 = _43 = 0.0f;
	}
	inline matrix4x4(float f11, float f12, float f13, float f14,
				 	 float f21, float f22, float f23, float f24,
					 float f31, float f32, float f33, float f34,
					 float f41, float f42, float f43, float f44)
	{
		_11 = f11; _12 = f12; _13 = f13; _14 = f14;
		_21 = f21; _22 = f22; _23 = f23; _24 = f24;
		_31 = f31; _32 = f32; _33 = f33; _34 = f34;
		_41 = f41; _42 = f42; _43 = f43; _44 = f44;
	}
	inline float* operator[] (int i)
	{
		return &(asArray[i * 4]);
	}
}matrix4x4;
// operator setup

matrix2x2 operator*(const matrix2x2& matrix,  float scalar);
matrix2x2 operator*(const matrix2x2& matrixA, const matrix2x2& matrixB);

matrix3x3 operator*(const matrix3x3& matrix, float scalar);
matrix3x3 operator*(const matrix3x3& matrixA, const matrix3x3& matrixB);

matrix4x4 operator*(const matrix4x4& matrix, float scalar);
matrix4x4 operator*(const matrix4x4& matrixA, const matrix4x4& matrixB);

#ifndef NO_EXTRAS
bool operator==(const matrix2x2& j, const matrix2x2& k);
bool operator!=(const matrix2x2& j, const matrix2x2& k);
std::ostream& operator<<(std::ostream& os, const matrix2x2& m);

bool operator==(const matrix3x3& j, const matrix3x3& k);
bool operator!=(const matrix3x3& j, const matrix3x3& k);
std::ostream& operator<<(std::ostream& os, const matrix3x3& m);

bool operator==(const matrix4x4& j, const matrix4x4& k);
bool operator!=(const matrix4x4& j, const matrix4x4& k);
std::ostream& operator<<(std::ostream& os, const matrix4x4& m);

#endif // !NO_EXTRAS

// other matrix stuffs 

void Transpose(const float* srcMat, float* dstMat, int srcRows, int srcCols);
bool Multiply(float* out, const float* matrixA, int aRows, int aCols, const float* matrixB, int bRows, int bCols);
void Cofactor(float* out, const float* minor, int rows, int cols);


matrix2x2 Transpose(const matrix2x2& matrix);
matrix2x2 Cut(const matrix3x3& matrix, int row, int col);
matrix2x2 Minor(const matrix2x2& matrix);
matrix2x2 Cofactor(const matrix2x2& matrix);
matrix2x2 Adjugate(const matrix2x2& matrix);
matrix2x2 Inverse(const matrix2x2& matrix);

float Determinant(const matrix2x2& matrix);

matrix3x3 Transpose(const matrix3x3& matrix);
matrix3x3 Cut(const matrix4x4& matrix, int row, int col);
matrix3x3 Minor(const matrix3x3& matrix);
matrix3x3 Cofactor(const matrix3x3& matrix);
matrix3x3 Adjugate(const matrix3x3& matrix);
matrix3x3 Inverse(const matrix3x3& matrix);
matrix3x3 ToColoumnMajor(const matrix3x3& matrix);
matrix3x3 FromColoumnMajor(const matrix3x3& matrix);
matrix3x3 Rotation3x3(float pitch, float yaw, float roll); // X, Y , Z
matrix3x3 Xrotation3x3(float angle);
matrix3x3 Yrotation3x3(float angle);
matrix3x3 Zrotation3x3(float angle);
matrix3x3 AxisAngle3x3(const Vector3& axis, float angle);

float Determinant(const matrix3x3& matrix);
Vector3 MultiplyVector(const Vector3& vec, const matrix3x3& matrix);
Vector3 Decompose(const matrix3x3& matrix);

matrix4x4 Transpose(const matrix4x4& matrix);
matrix4x4 Minor(const matrix4x4& matrix);
matrix4x4 Cofactor(const matrix4x4& matrix);
matrix4x4 Adjugate(const matrix4x4& matrix);
matrix4x4 Inverse(const matrix4x4& matrix);
matrix4x4 ToColoumnMajor(const matrix4x4& matrix);
matrix4x4 FromColoumnMajor(const matrix4x4& matrix);
matrix4x4 FromColoumnMajor(const float* matrix);
matrix4x4 Translation(float x, float y, float z);
matrix4x4 Translation(const Vector3& pos);
matrix4x4 FromMatrix3x3(const matrix3x3& matrix);
matrix4x4 Scale(float x, float y, float z);
matrix4x4 Scale(const Vector3& position);
matrix4x4 Rotation4x4(float pitch, float yaw, float roll); // X, Y , Z
matrix4x4 Xrotation4x4(float angle);
matrix4x4 Yrotation4x4(float angle);
matrix4x4 Zrotation4x4(float angle);
matrix4x4 AxisAngle4x4(const Vector3& axis, float angle);
matrix4x4 Transform(const Vector3& scale, const Vector3& eulerRotation, Vector3& translate);
matrix4x4 Transform(const Vector3& scale, const Vector3& rotationAxis, float rotationAngle, const Vector3& translate);
matrix4x4 LookAt(const Vector3& position, const Vector3& target, const Vector3& up);
matrix4x4 Projection(float fov, float aspect, float zNear, float zFar);
matrix4x4 Orthogninal(float left, float right, float bottom, float top, float zNear, float zFar);

float Determinant(const matrix4x4& matrix);
Vector3 GetTranslation(const matrix4x4& matrix);
Vector3 GetScale(const matrix4x4& matrix);
Vector3 MultiplyVector(const Vector3& vec, const matrix4x4& matrix);
Vector3 MultiplyPoint(const Vector3& vec, const matrix4x4& matrix);

#ifndef NO_EXTRAS

matrix2x2 Rotation2x2(float angle);

matrix3x3 Orthogonalize(const matrix3x3& matrix);
matrix3x3 FastInverse(const matrix3x3& matrix);

matrix4x4 Translate(float x, float y, float z);
matrix4x4 Translate(const Vector3& pos);
matrix4x4 YawPitchRoll(float yaw, float pitch, float roll);
matrix4x4 Orthogonalize(const matrix4x4& matrix);
matrix4x4 FastInverse(const matrix4x4& matrix);

#endif // !NO_EXTRAS

#endif // !_H_MATH_MARTICES_
