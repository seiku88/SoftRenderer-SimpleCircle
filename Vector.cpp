
#include "stdafx.h"
#include "Vector.h"

float Vector2::DistNonSquared(const Vector2 &a, const Vector2 &b)
{
	return (b.x - a.x)*(b.x - a.x) + (b.y - a.y)*(b.y - a.y);
}

float Vector2::Dist(const Vector2 &a, const Vector2 &b)
{
	return sqrtf(DistNonSquared(a, b));
}