#pragma once

#include "stdafx.h"
#include "Vector.h"

struct Vertex
{
public:
	Vertex() {}
	Vector3 position;
	ULONG colour;
};

class Triangle
{
public:
	IntPoint minPos, maxPos;
	Vertex vertices[3];

	Triangle() {}

	Triangle operator *(const Matrix3 other);

	void CalculateMinMaxPos();
	bool IsInArea(IntPoint pos);
	ULONG GetPointColour(IntPoint pos);
};

