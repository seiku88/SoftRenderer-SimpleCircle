#include "stdafx.h"
#include "Triangle.h"

Triangle Triangle::operator *(const Matrix3 other)
{
	Triangle tmp;
	for (int i = 0; i < 3; i++)
	{
		tmp.vertices[i].position = vertices[i].position * other;
	}
	return tmp;
}

void Triangle::CalculateMinMaxPos()
{
	minPos.x = minPos.y = INFINITY;
	maxPos.x = maxPos.y = -INFINITY;
	for (int i = 0; i < 3; i++)
	{
		if (vertices[i].position.x < minPos.x) minPos.x = vertices[i].position.x;
		if (vertices[i].position.x > maxPos.x) maxPos.x = vertices[i].position.x;
		if (vertices[i].position.y < minPos.y) minPos.y = vertices[i].position.y;
		if (vertices[i].position.y > maxPos.y) maxPos.y = vertices[i].position.y;
	}

	u = (vertices[1].position - vertices[0].position).ToVector2();
	v = (vertices[2].position - vertices[0].position).ToVector2();
	dotUU = Vector2::Dot(u, u);
	dotUV = Vector2::Dot(u, v);
	dotVV = Vector2::Dot(v, v);
	invDenom = 1.0f / (dotUU * dotVV - dotUV * dotUV);
}
