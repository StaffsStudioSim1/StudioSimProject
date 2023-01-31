#pragma once
#include <cmath>
#include <cfloat>


#ifndef _H_MATH_VECTORRS_
#define _H_MATH_VECTORS_

#ifndef NO_EXTRAS
#include <ostream>
#endif // !NO_EXTRAS

#ifndef  RAD2DEG
float RAD2DEG(float radians);
#endif // ! RAD2DEG
#ifndef DEG2RAD
float DEG2RAD(float degrees);
#endif // !DEG2RAD
float CorrectDegrees(float degrees);



typedef struct Vector2
{
	union
	{
		struct
		{
			float x;
			float y;
		};
		float asArray[2];
	};

	inline float& operator[](int i)
	{
		return asArray[i];
	}

	inline Vector2() : x(0.0f), y(0.0f) { }
	inline Vector2(float _x, float _y) : x(_x), y(_y) { }

} Vector2;

typedef struct Vector3
{
	union
	{
		struct
		{
			float x;
			float y;
			float z;
		};
		float asArray[3];
	};

	inline float& operator[](int i)
	{
		return asArray[i];
	}

	inline Vector3() : x(0.0f), y(0.0f), z(0.0f) { }
	inline Vector3(float _x, float _y, float _z) : x(_x), y(_y), z(_z) { }

} Vector3;






Vector2 operator+(const Vector2& j, const Vector2& k);
Vector2 operator-(const Vector2& j, const Vector2& k);
Vector2 operator*(const Vector2& j, const Vector2& k);
Vector2 operator*(const Vector2& j, float k);
bool operator==(const Vector2& j, const Vector2& k);
bool operator!=(const Vector2& j, const Vector2& k);

Vector3 operator+(const Vector3& j, const Vector3& k);
Vector3 operator-(const Vector3& j, const Vector3& k);
Vector3 operator*(const Vector3& j, const Vector3& k);
Vector3 operator*(const Vector3& j, float k);
bool operator==(const Vector3& j, const Vector3& k);
bool operator!=(const Vector3& j, const Vector3& k);

#ifndef NO_EXTRAS
Vector2 operator/(const Vector2& j, const Vector2& k);
Vector2 operator/(const Vector2& j, float k);
Vector2& operator+=(Vector2& j, const Vector2& k);
Vector2& operator-=(Vector2& j, const Vector2& k);
Vector2& operator*=(Vector2& j, const Vector2& k);
Vector2& operator*=(Vector2& j, const float k);
Vector2& operator/=(Vector2& j, const Vector2& k);
Vector2& operator/=(Vector2& j, const float k);
std::ostream& operator<<(std::ostream& os, const Vector2& m);

Vector3 operator/(const Vector3& j, const Vector3& k);
Vector3 operator/(const Vector3& j, float k);
Vector3& operator+=(Vector3& j, const Vector3& k);
Vector3& operator-=(Vector3& j, const Vector3& k);
Vector3& operator*=(Vector3& j, const Vector3& k);
Vector3& operator*=(Vector3& j, const float k);
Vector3& operator/=(Vector3& j, const Vector3& k);
Vector3& operator/=(Vector3& j, const float k);
std::ostream& operator<<(std::ostream& os, const Vector3& m);

#endif // !NO_EXTRAS

float Dot(const Vector2& j, const Vector2& k);
float Dot(const Vector3& j, const Vector3& k);

float Magnitude(const Vector2& v);
float Magnitude(const Vector3& v);

float MagnitudeSq(const Vector2& v);
float MagnitudeSq(const Vector3& v);

float Angle(const Vector2& j, const Vector2& k);
float Angle(const Vector3& j, const Vector3& k);

void Normalize(Vector2& v);
void Normalize(Vector3& v);

Vector2 Normalized(const Vector2& v);
Vector3 Normalized(const Vector3& v);

Vector2 GetNegVer(const Vector2& v);
Vector3 GetNegVer(const Vector3& v);

Vector2 Project(const Vector2& length, const Vector2& direction);
Vector3 Project(const Vector3& length, const Vector3& direction);

Vector2 Perpendicular(const Vector2 length, const Vector2 direction);
Vector3 Perpendicular(const Vector3 length, const Vector3 direction);

Vector2 Reflection(const Vector2& sourceVector, const Vector2& normal);
Vector3 Reflection(const Vector3& sourceVector, const Vector3& normal);

Vector3 Cross(const Vector3& j, const Vector3& k);

#ifndef NO_EXTRAS

float Distance(const Vector2& point1, const Vector2& point2);
float Distance(const Vector3& point1, const Vector3& point2);

float DistanceSq(const Vector2& point1, const Vector2& point2);
float DistanceSq(const Vector3& point1, const Vector3& point2);

Vector2 RotateVector(const Vector2& vector, float degrees);

#endif // !NO_EXTRAS


#endif // !_H_MATH_VECTORRS_