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
};