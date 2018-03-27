
#include "stdafx.h"
#include "Vector.h"

//V2
float Vector2::DistNonSquared(const Vector2 &a, const Vector2 &b)
{
	return (b.x - a.x)*(b.x - a.x) + (b.y - a.y)*(b.y - a.y);
}

float Vector2::Dist(const Vector2 &a, const Vector2 &b)
{
	return sqrtf(DistNonSquared(a, b));
}

float Vector2::Dot(const Vector2 &v)
{
	return v.x*v.x + v.y*v.y;
}

Vector2 Vector2::normalize(const Vector2 &v)// 1f / sqrtf(x*x+y*y)
{
	Vector2 newval(v.x / sqrtf(v.x*v.x + v.y*v.y), v.y / sqrtf(v.x*v.x + v.y*v.y));
	return newval;
}

Vector2 Vector2::operator +(const Vector2 &a, const Vector2 &b)
{
	Vector2 newval(a.x + b.x, a.y + b.y);
	return newval;
}

Vector2 Vector2::operator -(const Vector2 &a, const Vector2 &b)
{
	Vector2 newval(a.x - b.x, a.y - b.y);
	return newval;
}

Vector2 Vector2::operator /(const Vector2 &a, const Vector2 &b)
{
	Vector2 newval(a.x / b.x, a.y / b.y);
	return newval;
}

Vector2 Vector2::operator *(const Vector2 &a, const Vector2 &b)
{
	Vector2 result(a.x*b.x, a.y*b.y);
	return result;
}

Vector2 Vector2::operator *(const Matrix2 other) const
{
	Vector2 result;
	result.x = x*other._11 + y*other._12;
	result.y = x*other._21 + y*other._22;

	return result;
}

//V3

float Vector3::DistNonSquared(const Vector3 &a, const Vector3 &b)
{
	return (b.x - a.x)*(b.x - a.x) + (b.y - a.y)*(b.y - a.y);
}

float Vector3::Dist(const Vector3 &a, const Vector3 &b)
{
	return sqrtf(DistNonSquared(a, b));
}