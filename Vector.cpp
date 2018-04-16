
#include "stdafx.h"
#include "Vector.h"

//V2
float Vector2::DistNonSquared(const Vector2 &a, const Vector2 &b)
{
	return (b.x - a.x)*(b.x - a.x) + (b.y - a.y)*(b.y - a.y);
}

float Vector2::Dist(const Vector2 &a, const Vector2 &b)
{
	return sqrtf(Vector2::DistNonSquared(a, b));
}

float Vector2::Dot(const Vector2 &a, const Vector2 &b)
{
	return a.x*b.x + a.y*b.y;
}

Vector2 Vector2::normalize(const Vector2 &v)// 1f / sqrtf(x*x+y*y)
{
	return Vector2(v.x / sqrtf(v.x*v.x + v.y*v.y), v.y / sqrtf(v.x*v.x + v.y*v.y));
}

Vector2 Vector2::operator +(const Vector2 other) const
{
	return Vector2(x + other.x, y + other.y);
}

Vector2 Vector2::operator -(const Vector2 other) const
{
	return Vector2(x - other.x, y - other.y);
}

Vector2 Vector2::operator *(const Vector2 other) const
{
	return Vector2(x*other.x, y*other.y);
}

Vector2 Vector2::operator *(const Matrix2 other) const
{
	Vector2 result;
	result.x = x*other._11 + y*other._21;
	result.y = x*other._12 + y*other._22;

	return result;
}

Vector2 Vector2::operator /(const Vector2 other) const
{
	return Vector2(x / other.x, y / other.y);
}

Vector2& Vector2::operator +=(const Vector2 other)
{
	this->x += other.x;
	this->y += other.y;
	return *this;
}

Vector2& Vector2::operator -=(const Vector2 other)
{
	this->x += other.x;
	this->y += other.y;
	return *this;
}

Vector2& Vector2::operator *=(const Vector2 other)
{
	this->x += other.x;
	this->y += other.y;
	return *this;
}

Vector2& Vector2::operator /=(const Vector2 other)
{
	this->x += other.x;
	this->y += other.y;
	return *this;
}

//V3

float Vector3::DistNonSquared(const Vector3 &a, const Vector3 &b)
{
	return (b.x - a.x)*(b.x - a.x) + (b.y - a.y)*(b.y - a.y) + (b.z - a.z)*(b.z - a.z);
}

float Vector3::Dist(const Vector3 &a, const Vector3 &b)
{
	return sqrtf(Vector3::DistNonSquared(a, b));
}

float Vector3::Dot(const Vector3 &a, const Vector3 &b)
{
	return a.x * b.x + a.y * b.y + a.z * b.z;
}

Vector3 Vector3::operator +(const Vector3 other) const
{
	return Vector3(x + other.x, y + other.y, z + other.z);
}

Vector3 Vector3::operator -(const Vector3 other) const
{
	return Vector3(x - other.x, y - other.y, z - other.z);
}

Vector3 Vector3::operator *(const Vector3 other) const
{
	return Vector3(x*other.x, y*other.y, z * other.z);
}

Vector3 Vector3::operator *(const Matrix3 other) const
{
	Vector3 result;
	result.x = x * other._11 + y * other._12 + z * other._13;
	result.y = x * other._21 + y * other._22 + z * other._23;
	result.z = x * other._31 + y * other._32 + z * other._33;
	return result;
}

Vector3 Vector3::operator /(const Vector3 other) const
{
	return Vector3(x / other.x, y / other.y, z / other.z);
}

Vector3& Vector3::operator +=(const Vector3 other)
{
	this->x += other.x;
	this->y += other.y;
	this->z += other.z;
	return *this;
}

Vector3& Vector3::operator -=(const Vector3 other)
{
	this->x += other.x;
	this->y += other.y;
	this->z -= other.z;
	return *this;
}

Vector3& Vector3::operator *=(const Vector3 other)
{
	this->x += other.x;
	this->y += other.y;
	this->z *= other.z;
	return *this;
}

Vector3& Vector3::operator /=(const Vector3 other)
{
	this->x += other.x;
	this->y += other.y;
	this->z /= other.z;
	return *this;
}

Vector2 Vector3::ToVector2()
{
	return Vector2(x, y);
}