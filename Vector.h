#pragma once

#include "stdafx.h"
#include "Matrix.h"

struct Vector2
{
public:
	float x;
	float y;

public:
	Vector2() : x(0), y(0) {}
	Vector2(float nx, float ny) : x(nx), y(ny) {}

	static float DistNonSquared(const Vector2 &a, const Vector2 &b);
	static float Dist(const Vector2 &a, const Vector2 &b);
	static float Dot(const Vector2 &v);
	static Vector2 normalize(const Vector2 &v); // 1f / sqrtf(x*x+y*y)

	static Vector2 operator +(const Vector2 &a, const Vector2 &b);
	static Vector2 operator -(const Vector2 &a, const Vector2 &b);
	static Vector2 operator *(const Vector2 &a, const Vector2 &b);
	Vector2 operator *(const Matrix2 other) const;
	static Vector2 operator /(const Vector2 &a, const Vector2 &b);

	Vector2 operator +=(const Vector2 other) const;
	Vector2 operator -=(const Vector2 other) const;
	Vector2 operator *=(const Vector2 other) const;
	Vector2 operator /=(const Vector2 other) const;

	FORCEINLINE bool Equals(const Vector3 &v, float tolerance) const
	{
		return fabs(x - v.x) <= tolerance && fabs(y - v.y) <= tolerance;
	}
};

struct Vector3
{
public:
	float x;
	float y;
	float z;

public:
	Vector3() : x(0), y(0), z(0) {}
	Vector3(float nx, float ny, float nz) : x(nx), y(ny), z(nz) {}

	static float DistNonSquared(const Vector3 &a, const Vector3 &b);
	static float Dist(const Vector3 &a, const Vector3 &b);
	static float Dot();
	static Vector3 Cross();
	static Vector3 normalize(); // 1f / sqrtf(x*x+y*y)

	Vector3 operator +(const Vector3 other) const;
	Vector3 operator -(const Vector3 other) const;
	Vector3 operator *(const Vector3 other) const;
	Vector3 operator /(const Vector3 other) const;

	FORCEINLINE bool Equals(const Vector3 &v, float tolerance) const
	{
		return fabs(x-v.x) <= tolerance && fabs(y - v.y) <= tolerance && fabs(z - v.z) <= tolerance;
	}
};