#include "stdafx.h"
#include "Triangle.h"

Triangle Triangle::operator *(const Matrix3 other)
{
	Triangle tmp;
	for (int i = 0; i < 3; i++)
	{
		tmp.vertices[i].vertex = vertices[i].vertex * other;
	}
	return tmp;
}

void Triangle::CalculateMinMaxPos()
{
	minPos.x = minPos.y = INFINITY;
	maxPos.x = maxPos.y = -INFINITY;
	for (int i = 0; i < 3; i++)
	{
		if (vertices[i].vertex.x < minPos.x) minPos.x = RoundToInt(vertices[i].vertex.x);
		if (vertices[i].vertex.x > maxPos.x) maxPos.x = RoundToInt(vertices[i].vertex.x);
		if (vertices[i].vertex.y < minPos.y) minPos.y = RoundToInt(vertices[i].vertex.y);
		if (vertices[i].vertex.y > maxPos.y) maxPos.y = RoundToInt(vertices[i].vertex.y);
	}

	u = (vertices[1].vertex - vertices[0].vertex).ToVector2();
	v = (vertices[2].vertex - vertices[0].vertex).ToVector2();
	dotUU = Vector2::Dot(u, u);
	dotUV = Vector2::Dot(u, v);
	dotVV = Vector2::Dot(v, v);
	invDenom = 1.0f / (dotUU * dotVV - dotUV * dotUV);
}
