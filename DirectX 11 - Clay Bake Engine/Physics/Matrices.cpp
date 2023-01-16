#include "Matrices.h"

#ifdef DO_SANITY_TESTS
	#include <iostream>
	#include "Compare.h"
#else
	#define CMP(x , y) (fabsf((x) - (y)) <= FLT_EPSILON * fmaxf (1.0f, fmaxf( fabsf(x), fabsf(y))))
#endif // DO_SANITY_TESTS

#ifndef NO_EXTRAS
bool operator==(const matrix2x2& j, const matrix2x2& k)
{
	for (int i = 0; i < 4; ++i)
	{
		float a = j.asArray[i];
		float b = k.asArray[i];

		if (!CMP(a, b)) // idk why but it wont let me check the array directly and i spent too long trying to fix
		{
			return false;
		}
	}
	return true;
}

bool operator!=(const matrix2x2& j, const matrix2x2& k)
{
	return !(j == k);
}

std::ostream& operator<<(std::ostream& os, const matrix2x2& m)
{
	os << m._11 << ", " << m._12 << " \n"
		<< m._21 << ", " << m._22 << "\n";
	return os;
}

bool operator==(const matrix3x3& j, const matrix3x3& k)
{
	for (int i = 0; i < 9; ++i)
	{
		float a = j.asArray[i];
		float b = k.asArray[i];

		if (!CMP(a, b)) // idk why but it wont let me check the array directly and i spent too long trying to fix
		{
			return false;
		}
	}
	return true;
}

bool operator!=(const matrix3x3& j, const matrix3x3& k)
{
	return !(j == k);
}

std::ostream& operator<<(std::ostream& os, const matrix3x3& m)
{
	os << m._11 << ", " << m._12 << ", " << m._13 << " \n" 
	   << m._21 << ", " << m._22 << ", " << m._23 << " \n" 
	   << m._31 << ", " << m._32 << ", " << m._33 << "\n";
	return os;
}

bool operator==(const matrix4x4& j, const matrix4x4& k)
{
	for (int i = 0; i < 16; ++i)
	{
		float a = j.asArray[i];
		float b = k.asArray[i];

		if (!CMP(a, b)) // idk why but it wont let me check the array directly and i spent too long trying to fix
		{
			return false;
		}
	}
	return true;
}

bool operator!=(const matrix4x4& j, const matrix4x4& k)
{
	return !(j == k);
}

std::ostream& operator<<(std::ostream& os, const matrix4x4& m)
{
	os << m._11 << ", " << m._12 << ", " << m._13 << ", " << m._14 << "\n" 
	   << m._21 << ", " << m._22 << ", " << m._23 << ", " << m._24 << "\n"
	   << m._31 << ", " << m._32 << ", " << m._33 << ", " << m._34 << "\n" 
	   << m._41 << ", " << m._42 << ", " << m._43 << ", " << m._44 << "\n";

	return os;
}

#endif // !NO_EXTRAS

matrix2x2 operator*(const matrix2x2& matrix, float scalar)
{
	matrix2x2 result;
	for (int i = 0; i < 4; ++i)
	{
		result.asArray[i] = matrix.asArray[i] * scalar;
	}
	return result;
}

matrix2x2 operator*(const matrix2x2& matrixA, const matrix2x2& matrixB)
{
	matrix2x2 result;
	Multiply(result.asArray, matrixA.asArray, 2, 2, matrixB.asArray, 2, 2);
	return result;
}

matrix3x3 operator*(const matrix3x3& matrix, float scalar)
{
	matrix3x3 result;
	for (int i = 0; i < 9; ++i)
	{
		result.asArray[i] = matrix.asArray[i] * scalar;
	}
	return result;
}

matrix3x3 operator*(const matrix3x3& matrixA, const matrix3x3& matrixB)
{
	matrix3x3 result;
	Multiply(result.asArray, matrixA.asArray, 3, 3, matrixB.asArray, 3, 3);
	return result;
}

matrix4x4 operator*(const matrix4x4& matrix, float scalar)
{
	matrix4x4 result;
	for (int i = 0; i < 16; ++i)
	{
		result.asArray[i] = matrix.asArray[i] * scalar;
	}
	return result;
}

matrix4x4 operator*(const matrix4x4& matrixA, const matrix4x4& matrixB)
{
	matrix4x4 result; 
	Multiply(result.asArray, matrixA.asArray, 4, 4, matrixB.asArray, 4, 4);
	return result;
}
// other matrix stuffs 
void Transpose(const float* srcMat, float* dstMat, int srcRows, int srcCols)
{
	for (int i = 0; i < srcRows * srcCols; i++)
	{
		int row = i / srcRows;
		int col = i % srcRows;
		dstMat[i] = srcMat[srcCols * col + row];
	}
}

bool Multiply(float* out, const float* matrixA, int aRows, int aCols, const float* matrixB, int bRows, int bCols)
{
	if (aCols != bRows)
	{
		return false;
	}
	for (int i = 0; i < aRows; i++)
	{
		for (int j = 0; j < bCols; ++j)
		{
			out[bCols * i + j] = 0.0f;
			for (int k = 0; k < bRows; k++)
			{
				out[bCols * i + j] += matrixA[aCols * i + k] * matrixB[bCols * k + j];
			}
		}
	}
	return true;
}

void Cofactor(float* out, const float* minor, int rows, int cols)
{
	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < cols; j++)
		{
			out[cols * j + i] = minor[cols * j + i] * powf(-1.0, i + j);
		}
	}
}

matrix2x2 Transpose(const matrix2x2& matrix)
{
	matrix2x2 result;
	Transpose(matrix.asArray, result.asArray, 2, 2);
	return result;
}

matrix2x2 Cut(const matrix3x3& matrix, int row, int col)
{
	matrix2x2 result;
	int index = 0;

	for (int i = 0; i < 3; ++i)
	{
		for (int j = 0; j < 3; ++j)
		{
			if (i == row || j == col)
			{
				continue;
			}
			result.asArray[index++] = matrix.asArray[3 * i + j];
		}
	}
	return result;
}

matrix2x2 Minor(const matrix2x2& matrix)
{
	return matrix2x2 (matrix._22, matrix._21, matrix._12, matrix._11);
}

matrix2x2 Cofactor(const matrix2x2& matrix)
{
	matrix2x2 result; 
	Cofactor(result.asArray, Minor(matrix).asArray, 3, 3);
	return result;
}

matrix2x2 Adjugate(const matrix2x2& matrix)
{
	return Transpose(Cofactor(matrix));
}

matrix2x2 Inverse(const matrix2x2& matrix)
{
	float det = Determinant(matrix);
	if (CMP(det, 0.0f)) { return matrix2x2(); }
	return Adjugate(matrix) * (1.0f / det);
}

float Determinant(const matrix2x2& matrix)
{
	return matrix._11 * matrix._22 - matrix._12 * matrix._21;
}

matrix3x3 Transpose(const matrix3x3& matrix)
{
	matrix3x3 result;
	Transpose(matrix.asArray, result.asArray, 3, 3);
	return result;
}

matrix3x3 Cut(const matrix4x4& matrix, int row, int col)
{
	matrix3x3 result;
	int index = 0;

	for (int i = 0; i < 4; ++i)
	{
		for (int j = 0; j < 4; ++j)
		{
			if (i == row || j == col)
			{
				continue;
			}
			result.asArray[index++] = matrix.asArray[3 * i + j];
		}
	}
	return result;
}

matrix3x3 Minor(const matrix3x3& matrix)
{
	matrix3x3 result;

	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			result[i][j] = Determinant(Cut(matrix, i, j));
		}
	}
	return result;
}

matrix3x3 Cofactor(const matrix3x3& matrix)
{
	matrix3x3 result; 
	Cofactor(result.asArray, Minor(matrix).asArray, 2, 2);

	return result;
}

matrix3x3 Adjugate(const matrix3x3& matrix)
{
	return Transpose(Cofactor(matrix));
}

matrix3x3 Inverse(const matrix3x3& matrix)
{
	float det = Determinant(matrix);
	if (CMP(det, 0.0f)) { return matrix3x3(); }
	return Adjugate(matrix) * (1.0f / det);
}

matrix3x3 ToColoumnMajor(const matrix3x3& matrix)
{
	return Transpose(matrix);
}

matrix3x3 FromColoumnMajor(const matrix3x3& matrix)
{
	return Transpose(matrix);
}

matrix3x3 Rotation3x3(float pitch, float yaw, float roll) // X, Y , Z
{
	return Zrotation3x3(roll) * Xrotation3x3(pitch) * Yrotation3x3(yaw);
}

matrix3x3 Xrotation3x3(float angle)
{
	angle = DEG2RAD(angle);
	return matrix3x3(
		1.0f, 0.0f, 0.0f,
		0.0f, cosf(angle), sinf(angle),
		0.0f, -sinf(angle), cosf(angle)
		);
}

matrix3x3 Yrotation3x3(float angle)
{
	angle = DEG2RAD(angle);
	return matrix3x3(
			cosf(angle), 0.0f, -sinf(angle),
			0.0f, 1.0f, 0.0f,
			sinf(angle), 0.0f, cosf(angle)
		);
}
matrix3x3 Zrotation3x3(float angle)
{
	angle = DEG2RAD(angle);
	return matrix3x3(
		cosf(angle), sinf(angle), 0.0f,
		-sinf(angle), cosf(angle), 0.0f,
		0.0f, 0.0f, 1.0f
	);
}

matrix3x3 AxisAngle3x3(const Vector3& axis, float angle)
{
	angle = DEG2RAD(angle);
	float c = cosf(angle);
	float s = sinf(angle);
	float t = 1.0f - cosf(angle);

	float x = axis.x;
	float y = axis.y;
	float z = axis.z;
	if (!CMP(MagnitudeSq(axis), 1.0f)) {
		float inv_len = 1.0f / Magnitude(axis);
		x *= inv_len;
		y *= inv_len;
		z *= inv_len;
	}

	return matrix3x3(
		t * (x * x) + c, t * x * y + s * z, t * x * z - s * y,
		t * x * y - s * z, t * (y * y) + c, t * y * z + s * x,
		t * x * z + s * y, t * y * z - s * x, t * (z * z) + c
	);
}

float Determinant(const matrix3x3& matrix)
{
	float result = 0.0f;

	matrix3x3 cofactor = Cofactor(matrix);
	for (int i = 0; i < 3; i++)
	{
		result += matrix.asArray[3 * 0 + i] * cofactor[0][i];
	}
	return result;
}

Vector3 MultiplyVector(const Vector3& vec, const matrix3x3& matrix)
{
	Vector3 result;
	result.x = Dot(vec, Vector3{ matrix._11, matrix._21, matrix._31 });
	result.y = Dot(vec, Vector3{ matrix._12, matrix._22, matrix._32 });
	result.z = Dot(vec, Vector3{ matrix._13, matrix._23, matrix._33 });
	return result;
}

Vector3 Decompose(const matrix3x3& matrix)
{
	matrix3x3 rot = Transpose(matrix);

	float sy = sqrt(rot._11 * rot._11 + rot._21 * rot._21);
	bool singular = sy < 1e-6; // If
	float x, y, z;
	if (!!singular)
	{
		x = atan2(rot._32, rot._33);
		y = atan2(-rot._31, sy);
		z = atan2(rot._21, rot._11);
	}
	else
	{
		x = atan2(-rot._23, rot._22);
		y = atan2(-rot._31, sy);
		z = 0;
	}

	return Vector3(x, y, z);
}

matrix4x4 Transpose(const matrix4x4& matrix)
{
	matrix4x4 result;
	Transpose(matrix.asArray, result.asArray, 3, 3);
	return result;
}

matrix4x4 Minor(const matrix4x4& matrix)
{
	matrix4x4 result; 

	for (int i = 0; i < 4; ++i)
	{
		for (int j = 0; j < 4; j++)
		{
			result[i][j] = Determinant(Cut(matrix, i, j));
		}
	}
	return result;
}

matrix4x4 Cofactor(const matrix4x4& matrix)
{
	matrix4x4 result;
	Cofactor(result.asArray, Minor(matrix).asArray, 4, 4);
	return result;
}

matrix4x4 Adjugate(const matrix4x4& matrix)
{
	return Transpose(Cofactor(matrix));
}

matrix4x4 Inverse(const matrix4x4& matrix)
{
	float det
		= matrix._11 * matrix._22 * matrix._33 * matrix._44 + matrix._11 * matrix._23 * matrix._34 * matrix._42 + matrix._11 * matrix._24 * matrix._32 * matrix._43
		+ matrix._12 * matrix._21 * matrix._34 * matrix._43 + matrix._12 * matrix._23 * matrix._31 * matrix._44 + matrix._12 * matrix._24 * matrix._33 * matrix._41
		+ matrix._13 * matrix._21 * matrix._32 * matrix._44 + matrix._13 * matrix._22 * matrix._34 * matrix._41 + matrix._13 * matrix._24 * matrix._31 * matrix._42
		+ matrix._14 * matrix._21 * matrix._33 * matrix._42 + matrix._14 * matrix._22 * matrix._31 * matrix._43 + matrix._14 * matrix._23 * matrix._32 * matrix._41
		- matrix._11 * matrix._22 * matrix._34 * matrix._43 - matrix._11 * matrix._23 * matrix._32 * matrix._44 - matrix._11 * matrix._24 * matrix._33 * matrix._42
		- matrix._12 * matrix._21 * matrix._33 * matrix._44 - matrix._12 * matrix._23 * matrix._34 * matrix._41 - matrix._12 * matrix._24 * matrix._31 * matrix._43
		- matrix._13 * matrix._21 * matrix._34 * matrix._42 - matrix._13 * matrix._22 * matrix._31 * matrix._44 - matrix._13 * matrix._24 * matrix._32 * matrix._41
		- matrix._14 * matrix._21 * matrix._32 * matrix._43 - matrix._14 * matrix._22 * matrix._33 * matrix._41 - matrix._14 * matrix._23 * matrix._31 * matrix._42;

	if (CMP(det, 0.0f)) {
		return matrix4x4();						// doing it like this is more efficient than the function call way for some reason 
	}
	float i_det = 1.0f / det;

	matrix4x4 result;
	result._11 = (matrix._22 * matrix._33 * matrix._44 + matrix._23 * matrix._34 * matrix._42 + matrix._24 * matrix._32 * matrix._43 - matrix._22 * matrix._34 * matrix._43 - matrix._23 * matrix._32 * matrix._44 - matrix._24 * matrix._33 * matrix._42) * i_det;
	result._12 = (matrix._12 * matrix._34 * matrix._43 + matrix._13 * matrix._32 * matrix._44 + matrix._14 * matrix._33 * matrix._42 - matrix._12 * matrix._33 * matrix._44 - matrix._13 * matrix._34 * matrix._42 - matrix._14 * matrix._32 * matrix._43) * i_det;
	result._13 = (matrix._12 * matrix._23 * matrix._44 + matrix._13 * matrix._24 * matrix._42 + matrix._14 * matrix._22 * matrix._43 - matrix._12 * matrix._24 * matrix._43 - matrix._13 * matrix._22 * matrix._44 - matrix._14 * matrix._23 * matrix._42) * i_det;
	result._14 = (matrix._12 * matrix._24 * matrix._33 + matrix._13 * matrix._22 * matrix._34 + matrix._14 * matrix._23 * matrix._32 - matrix._12 * matrix._23 * matrix._34 - matrix._13 * matrix._24 * matrix._32 - matrix._14 * matrix._22 * matrix._33) * i_det;
	result._21 = (matrix._21 * matrix._34 * matrix._43 + matrix._23 * matrix._31 * matrix._44 + matrix._24 * matrix._33 * matrix._41 - matrix._21 * matrix._33 * matrix._44 - matrix._23 * matrix._34 * matrix._41 - matrix._24 * matrix._31 * matrix._43) * i_det;
	result._22 = (matrix._11 * matrix._33 * matrix._44 + matrix._13 * matrix._34 * matrix._41 + matrix._14 * matrix._31 * matrix._43 - matrix._11 * matrix._34 * matrix._43 - matrix._13 * matrix._31 * matrix._44 - matrix._14 * matrix._33 * matrix._41) * i_det;
	result._23 = (matrix._11 * matrix._24 * matrix._43 + matrix._13 * matrix._21 * matrix._44 + matrix._14 * matrix._23 * matrix._41 - matrix._11 * matrix._23 * matrix._44 - matrix._13 * matrix._24 * matrix._41 - matrix._14 * matrix._21 * matrix._43) * i_det;
	result._24 = (matrix._11 * matrix._23 * matrix._34 + matrix._13 * matrix._24 * matrix._31 + matrix._14 * matrix._21 * matrix._33 - matrix._11 * matrix._24 * matrix._33 - matrix._13 * matrix._21 * matrix._34 - matrix._14 * matrix._23 * matrix._31) * i_det;
	result._31 = (matrix._21 * matrix._32 * matrix._44 + matrix._22 * matrix._34 * matrix._41 + matrix._24 * matrix._31 * matrix._42 - matrix._21 * matrix._34 * matrix._42 - matrix._22 * matrix._31 * matrix._44 - matrix._24 * matrix._32 * matrix._41) * i_det;
	result._32 = (matrix._11 * matrix._34 * matrix._42 + matrix._12 * matrix._31 * matrix._44 + matrix._14 * matrix._32 * matrix._41 - matrix._11 * matrix._32 * matrix._44 - matrix._12 * matrix._34 * matrix._41 - matrix._14 * matrix._31 * matrix._42) * i_det;
	result._33 = (matrix._11 * matrix._22 * matrix._44 + matrix._12 * matrix._24 * matrix._41 + matrix._14 * matrix._21 * matrix._42 - matrix._11 * matrix._24 * matrix._42 - matrix._12 * matrix._21 * matrix._44 - matrix._14 * matrix._22 * matrix._41) * i_det;
	result._34 = (matrix._11 * matrix._24 * matrix._32 + matrix._12 * matrix._21 * matrix._34 + matrix._14 * matrix._22 * matrix._31 - matrix._11 * matrix._22 * matrix._34 - matrix._12 * matrix._24 * matrix._31 - matrix._14 * matrix._21 * matrix._32) * i_det;
	result._41 = (matrix._21 * matrix._33 * matrix._42 + matrix._22 * matrix._31 * matrix._43 + matrix._23 * matrix._32 * matrix._41 - matrix._21 * matrix._32 * matrix._43 - matrix._22 * matrix._33 * matrix._41 - matrix._23 * matrix._31 * matrix._42) * i_det;
	result._42 = (matrix._11 * matrix._32 * matrix._43 + matrix._12 * matrix._33 * matrix._41 + matrix._13 * matrix._31 * matrix._42 - matrix._11 * matrix._33 * matrix._42 - matrix._12 * matrix._31 * matrix._43 - matrix._13 * matrix._32 * matrix._41) * i_det;
	result._43 = (matrix._11 * matrix._23 * matrix._42 + matrix._12 * matrix._21 * matrix._43 + matrix._13 * matrix._22 * matrix._41 - matrix._11 * matrix._22 * matrix._43 - matrix._12 * matrix._23 * matrix._41 - matrix._13 * matrix._21 * matrix._42) * i_det;
	result._44 = (matrix._11 * matrix._22 * matrix._33 + matrix._12 * matrix._23 * matrix._31 + matrix._13 * matrix._21 * matrix._32 - matrix._11 * matrix._23 * matrix._32 - matrix._12 * matrix._21 * matrix._33 - matrix._13 * matrix._22 * matrix._31) * i_det;

#ifdef  DO_SANITY_TESTS
#ifndef NO_EXTRAS
	if (result * m != matrix4x4()) {
		std::cout << "ERROR! Expecting matrix x inverse to equal identity!\n";
	}
#endif 
#endif

	return result;
}

matrix4x4 ToColoumnMajor(const matrix4x4& matrix)
{
	return Transpose(matrix);
}

matrix4x4 FromColoumnMajor(const matrix4x4& matrix)
{
	return Transpose(matrix);
}

matrix4x4 FromColoumnMajor(const float* matrix)
{
	return Transpose(matrix4x4(
		matrix[0], matrix[1], matrix[2], matrix[3],
		matrix[4], matrix[5], matrix[6], matrix[7],
		matrix[8], matrix[9], matrix[10],matrix[11],
		matrix[12],matrix[13],matrix[14],matrix[15]
	));
}

matrix4x4 Translation(float x, float y, float z)
{
	return matrix4x4(1.0f, 0.0f, 0.0f, 0.0f,
					 0.0f, 1.0f, 0.0f, 0.0f,
					 0.0f, 0.0f, 1.0f, 0.0f,
					 x, y, z, 1.0f);
}

matrix4x4 Translation(const Vector3& pos)
{
	return matrix4x4(
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		pos.x, pos.y, pos.z, 1.0f
	);
}

matrix4x4 FromMatrix3x3(const matrix3x3& matrix)
{
	matrix4x4 result; 
	result._11 = matrix._11;
	result._12 = matrix._12;
	result._13 = matrix._13;

	result._21 = matrix._21;
	result._22 = matrix._22;
	result._23 = matrix._23;

	result._31 = matrix._31;
	result._32 = matrix._32;
	result._33 = matrix._33;

	return result;
}
matrix4x4 Scale(float x, float y, float z)
{
	return matrix4x4(
		x, 0.0f, 0.0f, 0.0f,
		0.0f, y, 0.0f, 0.0f,
		0.0f, 0.0f, z, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	);
}

matrix4x4 Scale(const Vector3& position)
{
	return matrix4x4(
		position.x, 0.0f, 0.0f, 0.0f,
		0.0f, position.y, 0.0f, 0.0f,
		0.0f, 0.0f, position.z, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	);
}

matrix4x4 Rotation4x4(float pitch, float yaw, float roll) // X, Y , Z
{
	return  Zrotation4x4(roll) * Xrotation4x4(pitch) * Yrotation4x4(yaw);
}

matrix4x4 Xrotation4x4(float angle)
{
	angle = DEG2RAD(angle);
	return matrix4x4(
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, cosf(angle), sinf(angle), 0.0f,
		0.0f, -sinf(angle), cosf(angle), 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	);
}

matrix4x4 Yrotation4x4(float angle)
{
	angle = DEG2RAD(angle);
	return matrix4x4(
		cosf(angle), 0.0f, -sinf(angle), 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		sinf(angle), 0.0f, cosf(angle), 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	);
}

matrix4x4 Zrotation4x4(float angle)
{
	angle = DEG2RAD(angle);
	return matrix4x4(
		cosf(angle), sinf(angle), 0.0f, 0.0f,
		-sinf(angle), cosf(angle), 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	);
}

matrix4x4 AxisAngle4x4(const Vector3& axis, float angle)
{
	angle = DEG2RAD(angle);
	float c = cosf(angle);
	float s = sinf(angle);
	float t = 1.0f - cosf(angle);

	float x = axis.x;
	float y = axis.y;
	float z = axis.z;
	if (!CMP(MagnitudeSq(axis), 1.0f)) {
		float inv_len = 1.0f / Magnitude(axis);
		x *= inv_len;
		y *= inv_len;
		z *= inv_len;
	}

	return matrix4x4(
		t * (x * x) + c, t * x * y + s * z, t * x * z - s * y, 0.0f,
		t * x * y - s * z, t * (y * y) + c, t * y * z + s * x, 0.0f,
		t * x * z + s * y, t * y * z - s * x, t * (z * z) + c, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	);
}

matrix4x4 Transform(const Vector3& scale, const Vector3& eulerRotation, Vector3& translate)
{
	return Scale(scale) *
		Rotation4x4(eulerRotation.x, eulerRotation.y, eulerRotation.z) *
		Translation(translate);
}

matrix4x4 Transform(const Vector3& scale, const Vector3& rotationAxis, float rotationAngle, const Vector3& translate)
{
	return Scale(scale) *
		AxisAngle4x4(rotationAxis, rotationAngle) *
		Translation(translate);
}

matrix4x4 LookAt(const Vector3& position, const Vector3& target, const Vector3& up)
{
	Vector3 forward = Normalized(target - position);
	Vector3 right = Normalized(Cross(up, forward));
	Vector3 newUp = Cross(forward, right);

#ifdef DO_SANITY_TESTS
	matrix4x4 viewPosition = Translation(position);
	matrix4x4 viewOrientation = matrix4x4(
		right.x, right.y, right.z, 0.0f,
		newUp.x, newUp.y, newUp.z, 0.0f,
		forward.x, forward.y, forward.z, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	);
	matrix4x4 view = Inverse(viewOrientation * viewPosition);
	matrix4x4 result =
#else
	return
#endif

		matrix4x4(
			right.x, newUp.x, forward.x, 0.0f,
			right.y, newUp.y, forward.y, 0.0f,
			right.z, newUp.z, forward.z, 0.0f,
			-Dot(right, position), -Dot(newUp, position), -Dot(forward, position), 1.0f
		);
#ifdef DO_SANITY_TESTS
#ifndef NO_EXTRAS
	if (result != view) {
		std::cout << "Error, result and view do not match in an expected manner!\n";
		std::cout << "view: \n" << view << "\n\n";
		std::cout << "result: \n" << result << "\n\n";
	}
#endif 
	return result;
#endif
}

matrix4x4 Projection(float fov, float aspect, float zNear, float zFar)
{
	float tanHalfFov = tanf(DEG2RAD((fov * 0.5f)));

	matrix4x4 result; // There are MANY different ways to derive a projection matrix!

#if 0
result._11 = 1.0f / (aspect * tanHalfFov);
result._22 = 1.0f / tanHalfFov;
result._33 = (-zNear - zFar) / (zNear - zFar);
result._44 = 0.0f;
result._34 = 1.0f;
result._43 = (2.0f * zFar * zNear) / (zNear - zFar);
#else
float fovY = 1.0f / tanHalfFov; // cot(fov/2)
float fovX = fovY / aspect; // cot(fov/2) / aspect

result._11 = fovX;
result._22 = fovY;
result._33 = zFar / (zFar - zNear); // far / range
result._34 = 1.0f;
result._43 = -zNear * result._33; // - near * (far / range)
result._44 = 0.0f;
#endif

return result;

}

matrix4x4 Orthogninal(float left, float right, float bottom, float top, float zNear, float zFar)
{
	float _11 = 2.0f / (right - left);
	float _22 = 2.0f / (top - bottom);
	float _33 = 1.0f / (zFar - zNear);
	float _41 = (left + right) / (left - right);
	float _42 = (top + bottom) / (bottom - top);
	float _43 = (zNear) / (zNear - zFar);

	return matrix4x4(
		_11, 0.0f, 0.0f, 0.0f,
		0.0f, _22, 0.0f, 0.0f,
		0.0f, 0.0f, _33, 0.0f,
		_41, _42, _43, 1.0f
	);
}

float Determinant(const matrix4x4& matrix)
{
	float result = 0.0f;

	matrix4x4 cofactor = Cofactor(matrix);
	for (int i = 0; i < 4; ++i) {
		result += matrix.asArray[4 * 0 + i] * cofactor[0][i];
	}

	return result;
}

Vector3 GetTranslation(const matrix4x4& matrix)
{
	return Vector3(matrix._41, matrix._42, matrix._43);
}

Vector3 GetScale(const matrix4x4& matrix)
{
	return Vector3(matrix._11, matrix._22, matrix._33);
}

Vector3 MultiplyVector(const Vector3& vec, const matrix4x4& matrix)
{
	Vector3 result;
	result.x = vec.x * matrix._11 + vec.y * matrix._21 + vec.z * matrix._31 + 0.0f * matrix._41;
	result.y = vec.x * matrix._12 + vec.y * matrix._22 + vec.z * matrix._32 + 0.0f * matrix._42;
	result.z = vec.x * matrix._13 + vec.y * matrix._23 + vec.z * matrix._33 + 0.0f * matrix._43;
	return result;
}

Vector3 MultiplyPoint(const Vector3& vec, const matrix4x4& matrix)
{
	Vector3 result;
	result.x = vec.x * matrix._11 + vec.y * matrix._21 + vec.z * matrix._31 + 1.0f * matrix._41;
	result.y = vec.x * matrix._12 + vec.y * matrix._22 + vec.z * matrix._32 + 1.0f * matrix._42;
	result.z = vec.x * matrix._13 + vec.y * matrix._23 + vec.z * matrix._33 + 1.0f * matrix._43;
	return result;
}

#ifndef NO_EXTRAS

matrix2x2 Rotation2x2(float angle)
{
	return matrix2x2(
		cosf(angle), sinf(angle),
		-sinf(angle), cosf(angle)
	);
}

matrix3x3 Orthogonalize(const matrix3x3& matrix)
{
	Vector3 xAxis(matrix._11, matrix._12, matrix._13);
	Vector3 yAxis(matrix._21, matrix._22, matrix._23);
	Vector3 zAxis = Cross(xAxis, yAxis);

	xAxis = Cross(yAxis, zAxis);
	yAxis = Cross(zAxis, xAxis);
	zAxis = Cross(xAxis, yAxis);

	return matrix3x3(
		xAxis.x, xAxis.y, xAxis.z,
		yAxis.x, yAxis.y, yAxis.z,
		zAxis.x, zAxis.y, zAxis.z
	);
}

matrix3x3 FastInverse(const matrix3x3& matrix)
{
	return Transpose(matrix);
}

matrix4x4 Translate(float x, float y, float z)
{
	return matrix4x4(
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		x, y, z, 1.0f
	);
}

matrix4x4 Translate(const Vector3& pos)
{
	return matrix4x4(
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		pos.x, pos.y, pos.z, 1.0f
	);
}

matrix4x4 YawPitchRoll(float yaw, float pitch, float roll)
{
	yaw = DEG2RAD(yaw);
	pitch = DEG2RAD(pitch);
	roll = DEG2RAD(roll);

	matrix4x4 out; // z * x * y
	out._11 = (cosf(roll) * cosf(yaw)) + (sinf(roll) * sinf(pitch) * sinf(yaw));
	out._12 = (sinf(roll) * cosf(pitch));
	out._13 = (cosf(roll) * -sinf(yaw)) + (sinf(roll) * sinf(pitch) * cosf(yaw));
	out._21 = (-sinf(roll) * cosf(yaw)) + (cosf(roll) * sinf(pitch) * sinf(yaw));
	out._22 = (cosf(roll) * cosf(pitch));
	out._23 = (sinf(roll) * sinf(yaw)) + (cosf(roll) * sinf(pitch) * cosf(yaw));
	out._31 = (cosf(pitch) * sinf(yaw));
	out._32 = -sinf(pitch);
	out._33 = (cosf(pitch) * cosf(yaw));
	out._44 = 1;
	return out;
}

matrix4x4 Orthogonalize(const matrix4x4& matrix)
{
	Vector3 xAxis(matrix._11, matrix._12, matrix._13);
	Vector3 yAxis(matrix._21, matrix._22, matrix._23);
	Vector3 zAxis = Cross(xAxis, yAxis);

	xAxis = Cross(yAxis, zAxis);
	yAxis = Cross(zAxis, xAxis);
	zAxis = Cross(xAxis, yAxis);

	return matrix4x4(
		xAxis.x, xAxis.y, xAxis.z, matrix._14,
		yAxis.x, yAxis.y, yAxis.z, matrix._24,
		zAxis.x, zAxis.y, zAxis.z, matrix._34,
		matrix._41, matrix._42, matrix._43, matrix._44
	);
}

matrix4x4 FastInverse(const matrix4x4& matrix)
{
	matrix4x4 inverse = Transpose(matrix);
	inverse._41 = inverse._14 = 0.0f;
	inverse._42 = inverse._24 = 0.0f;
	inverse._43 = inverse._34 = 0.0f;

	Vector3 right = Vector3(matrix._11, matrix._12, matrix._13);
	Vector3 up = Vector3(matrix._21, matrix._22, matrix._23);
	Vector3 forward = Vector3(matrix._31, matrix._32, matrix._33);
	Vector3 position = Vector3(matrix._41, matrix._42, matrix._43);

	inverse._41 = -Dot(right, position);
	inverse._42 = -Dot(up, position);
	inverse._43 = -Dot(forward, position);

	return inverse;
}

#endif // !NO_EXTRAS

