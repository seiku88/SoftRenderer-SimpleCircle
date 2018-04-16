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
}

bool Triangle::IsInArea(IntPoint pos)
{
	Vector2 u = (vertices[1].position - vertices[0].position).ToVector2();
	Vector2 v = (vertices[2].position - vertices[0].position).ToVector2();
	float dotUU = Vector2::Dot(u, u);
	float dotUV = Vector2::Dot(u, v);
	float dotVV = Vector2::Dot(v, v);
	float invDenom = 1.0f / (dotUU * dotVV - dotUV * dotUV);

	Vector2 w = (Vector3((float)pos.x, (float)pos.y, 0.0f) - vertices[0].position).ToVector2();
	float dotUW = Vector2::Dot(u, w);
	float dotVW = Vector2::Dot(v, w);
	float outS = (dotVV * dotUW - dotUV * dotVW) * invDenom;
	float outT = (dotUU * dotVW - dotUV * dotUW) * invDenom;
	if (outS >= 0.0f)
	{
		if (outT >= 0.0f)
		{
			if (outS + outT <= 1.0f) return true;
		}
	}

	return false;
}

ULONG Triangle::GetPointColour(IntPoint pos)
{
	ULONG interpolateA = vertices[0].colour, interpolateB = vertices[0].colour;
	ULONG result = vertices[0].colour;

	float ratio = vertices[0].position.y - (float)pos.y / (vertices[0].position.y-vertices[1].position.y);
	interpolateA = vertices[0].colour + RGB((GetRValue(vertices[1].colour)-GetRValue(vertices[0].colour))*ratio, (GetGValue(vertices[1].colour)-GetGValue(vertices[0].colour))*ratio, (GetBValue(vertices[1].colour)-GetBValue(vertices[0].colour))*ratio);

	return interpolateA;
}
