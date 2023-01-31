#include "Vectors.h"


#ifdef DO_SANITY_TESTS
#include <iostream>
#include "Compare.h"
#else
#define CMP(x , y)  (fabsf((x) - (y)) <= FLT_EPSILON * fmaxf (1.0f, fmaxf( fabsf(x), fabsf(y))))
#endif // DO_SANITY_TESTS

float CorrectDegrees(float degrees)
{
	while (degrees > 360.0f)
	{
		degrees -= 360.0f;
	}
	while (degrees < -360.0f)
	{
		degrees += 360.0f;
	}
	return degrees;
}

#ifndef RAD2DEG
float RAD2DEG(float radians)
{
	float degrees = radians * 57.295754f;
	degrees = CorrectDegrees(degrees);
	return degrees;
}
#endif // !RAD2DEG
#ifndef DEG2RAD
float DEG2RAD(float degrees)
{
	degrees = CorrectDegrees(degrees);
	float radians = degrees * 0.0174533f;
	return radians;
}
#endif // !DEG2RAD

Vector2 operator+(const Vector2& j, const Vector2& k)
{
	return { j.x + k.x, j.y + k.y };
}

Vector2 operator-(const Vector2& j, const Vector2& k)
{
	return { j.x - k.x, j.y - k.y };
}

Vector2 operator*(const Vector2& j, const Vector2& k)
{
	return { j.x * k.x, j.y * k.y };
}

Vector2 operator*(const Vector2& j, float k)
{
	return { j.x * k, j.y * k };
}

bool operator==(const Vector2& j, const Vector2& k)
{
	return CMP(j.x, k.x) && CMP(j.y, k.y);
}

bool operator!=(const Vector2& j, const Vector2& k)
{
	return !(j == k);
}

Vector3 operator+(const Vector3& j, const Vector3& k)
{
	return { j.x + k.x, j.y + k.y, j.z + k.z };
}

Vector3 operator-(const Vector3& j, const Vector3& k)
{
	return { j.x - k.x, j.y - k.y, j.z - k.z };
}

Vector3 operator*(const Vector3& j, const Vector3& k)
{
	return { j.x * k.x, j.y * k.y, j.z * k.z };
}

Vector3 operator*(const Vector3& j, float k)
{
	return { j.x * k, j.y * k, j.z * k };
}

bool operator==(const Vector3& j, const Vector3& k)
{
	return CMP(j.x, k.x) && CMP(j.y, k.y) && CMP(j.z, k.z);
}

bool operator!=(const Vector3& j, const Vector3& k)
{
	return !(j == k);
}

#ifndef NO_EXTRAS

Vector2 operator/(const Vector2& j, const Vector2& k)
{
	return { j.x / k.x, j.y / k.y };
}

Vector2 operator/(const Vector2& j, float k)
{
	return { j.x / k, j.y / k };
}

Vector2& operator+=(Vector2& j, const Vector2& k)
{
	j.x += k.x;
	j.y += k.y;
	return j;
}

Vector2& operator-=(Vector2& j, const Vector2& k)
{
	j.x -= k.x;
	j.y -= k.y;
	return j;
}

Vector2& operator*=(Vector2& j, const Vector2& k)
{
	j.x *= k.x;
	j.y *= k.y;
	return j;
}

Vector2& operator*=(Vector2& j, const float k)
{
	j.x *= k;
	j.y *= k;
	return j;
}

Vector2& operator/=(Vector2& j, const Vector2& k)
{
	j.x /= k.x;
	j.y /= k.y;
	return j;
}

Vector2& operator/=(Vector2& j, const float k)
{
	j.x /= k;
	j.y /= k;
	return j;
}

std::ostream& operator<<(std::ostream& os, const Vector2& m)
{
	os << "(" << m.x << ", " << m.y << ")";
	return os;
}

// vector 3 in case its needed 
Vector3 operator/(const Vector3& j, const Vector3& k)
{
	return { j.x / k.x, j.y / k.y, j.z / k.z };
}

Vector3 operator/(const Vector3& j, float k)
{

	return { j.x / k, j.y / k, j.z / k };
}

Vector3& operator+=(Vector3& j, const Vector3& k)
{
	j.x += k.x;
	j.y += k.y;
	j.z += k.z;
	return j;
}

Vector3& operator-=(Vector3& j, const Vector3& k)
{
	j.x -= k.x;
	j.y -= k.y;
	j.z -= k.z;
	return j;
}

Vector3& operator*=(Vector3& j, const Vector3& k)
{
	j.x *= k.x;
	j.y *= k.y;
	j.z *= k.z;
	return j;
}

Vector3& operator*=(Vector3& j, const float k)
{
	j.x *= k;
	j.y *= k;
	j.z *= k;
	return j;
}

Vector3& operator/=(Vector3& j, const Vector3& k)
{
	j.x /= k.x;
	j.y /= k.y;
	j.z /= k.z;
	return j;
}
Vector3& operator/=(Vector3& j, const float k)
{
	j.x /= k;
	j.y /= k;
	j.z /= k;
	return j;
}

std::ostream& operator<<(std::ostream& os, const Vector3& m)
{
	os << "(" << m.x << ", " << m.y << ", " << m.z << ")";
	return os;
}

#endif // !NO_EXTRAS



float Dot(const Vector2& j, const Vector2& k)
{
	return j.x * k.x + j.y * k.y;
}

float Dot(const Vector3& j, const Vector3& k)
{
	return j.x * k.x + j.y * k.y + j.z * k.z;
}

float Magnitude(const Vector2& v)
{
	return sqrtf(Dot(v, v));
}

float Magnitude(const Vector3& v)
{
	return sqrtf(Dot(v, v));
}

float MagnitudeSq(const Vector2& v)
{
	return Dot(v, v);
}

float MagnitudeSq(const Vector3& v)
{
	return Dot(v, v);
}

float Angle(const Vector2& j, const Vector2& k)
{
	return acosf(Dot(j, k) / sqrtf(MagnitudeSq(j) * MagnitudeSq(k)));
}

float Angle(const Vector3& j, const Vector3& k)
{
	return acosf(Dot(j, k) / sqrtf(MagnitudeSq(j) * MagnitudeSq(k)));
}

void Normalize(Vector2& v)
{
	v = v * (1.0f / Magnitude(v));
}

void Normalize(Vector3& v)
{
	v = v * (1.0f / Magnitude(v));
}

Vector2 Normalized(const Vector2& v)
{
	return v * (1.0f / Magnitude(v));
}
Vector3 Normalized(const Vector3& v)
{
	return v * (1.0f / Magnitude(v));
}

Vector2 Project(const Vector2& length, const Vector2& direction)
{
	float dot = Dot(length, direction);
	float magSq = Magnitude(direction);
	return direction * (dot / magSq);
}

Vector3 Project(const Vector3& length, const Vector3& direction)
{
	float dot = Dot(length, direction);
	float magSq = Magnitude(direction);
	return direction * (dot / magSq);
}

Vector2 Perpendicular(const Vector2 length, const Vector2 direction)
{
	return length - Project(length, direction);
}
Vector3 Perpendicular(const Vector3 length, const Vector3 direction)
{
	return length - Project(length, direction);
}

Vector2 Reflection(const Vector2& sourceVector, const Vector2& normal)
{
	return sourceVector - normal * (Dot(sourceVector, normal) * 2.0f);
}

Vector3 Reflection(const Vector3& sourceVector, const Vector3& normal)
{
	return sourceVector - normal * (Dot(sourceVector, normal) * 2.0f);
}

Vector3 Cross(const Vector3& j, const Vector3& k)
{
	Vector3 result;
	result.x = j.y * k.z - j.z * k.y;
	result.y = j.z * k.x - j.x * k.z;
	result.z = j.x * k.y - j.y * k.x;
	return result;
}

#ifndef NO_EXTRAS

float Distance(const Vector2& point1, const Vector2& point2)
{
	return Magnitude(point1 - point2);
}

float Distance(const Vector3& point1, const Vector3& point2)
{
	return Magnitude(point1 - point2);
}

float DistanceSq(const Vector2& point1, const Vector2& point2)
{
	return MagnitudeSq(point1 - point2);
}

float DistanceSq(const Vector3& point1, const Vector3& point2)
{
	return MagnitudeSq(point1 - point2);
}

Vector2 RotateVector(const Vector2 vector, float degrees)
{
	degrees = DEG2RAD(degrees);
	float s = sinf(degrees);
	float c = cosf(degrees);

	return Vector2(vector.x * c - vector.y * s, vector.x * s + vector.y * c);
}

#endif // !NO_EXTRAS