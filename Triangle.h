#pragma once

#include "stdafx.h"
#include "Vertex.h"

struct appdata_full
{
	appdata_full() {};
	Vector3 vertex;  // The vertex position in model space.
	Vector2 texcoord; // The first UV coordinate.
	ULONG colour;     // Per-vertex color
};

struct v2f
{
	v2f() {}
	v2f(Vector3 v, Vector2 uv, ULONG col) : vertex(v), texcoord(uv), colour(col) {}
	Vector3 vertex;  // The vertex position in model space.
	Vector2 texcoord; // The first UV coordinate.
	ULONG colour;     // Per-vertex color
};

class Triangle
{
public:
	IntPoint minPos, maxPos;
	v2f vertices[3];

	Vector2 u, v, w;
	float dotUU, dotUV, dotVV;
	float invDenom;

	Triangle() {}
	Triangle(v2f vert1, v2f vert2, v2f vert3)
	{
		vertices[0] = vert1;
		vertices[1] = vert2;
		vertices[2] = vert3;

		CalculateMinMaxPos();
	}

	Triangle operator *(const Matrix3 other);

	void CalculateMinMaxPos();

	void CalcBaryCentricCoord(Vector3 target, float *outS, float *outT)
	{
		Vector2 w = (target - vertices[0].vertex).ToVector2();
		float dotUW = Vector2::Dot(u, w);
		float dotVW = Vector2::Dot(v, w);
		*outS = (dotVV * dotUW - dotUV * dotVW) * invDenom;
		*outT = (dotUU * dotVW - dotUV * dotUW) * invDenom;
	}

	bool IsInArea(float s, float t)
	{
		if (s < 0.0f) return false;
		if (t < 0.0f) return false;
		if (s + t > 1.0f) return false;

		return true;
	}

	Vector3 GetFragmentPos(Vector3 target, float s, float t)
	{
		Vector3 Pos0 = vertices[0].vertex;
		Vector3 Pos0ToPos1 = vertices[1].vertex - vertices[0].vertex;
		Vector3 Pos0ToPos2 = vertices[2].vertex - vertices[0].vertex;
		return Pos0 + Pos0ToPos1 * s + Pos0ToPos2 * t;
	}

	Vector2 GetFragmentUV(Vector3 target, float s, float t)
	{
		Vector2 UV0 = vertices[0].texcoord;
		Vector2 UV0ToUV1 = vertices[1].texcoord - vertices[0].texcoord;
		Vector2 UV0ToUV2 = vertices[2].texcoord - vertices[0].texcoord;
		return UV0 + UV0ToUV1 * s + UV0ToUV2 * t;
	}

	ULONG GetFragmentColor(Vector3 target, float s, float t)
	{
		BYTE RV0 = GetRValue(vertices[0].colour);
		BYTE RV1 = GetRValue(vertices[1].colour);
		BYTE RV2 = GetRValue(vertices[2].colour);

		BYTE GV0 = GetGValue(vertices[0].colour);
		BYTE GV1 = GetGValue(vertices[1].colour);
		BYTE GV2 = GetGValue(vertices[2].colour);

		BYTE BV0 = GetBValue(vertices[0].colour);
		BYTE BV1 = GetBValue(vertices[1].colour);
		BYTE BV2 = GetBValue(vertices[2].colour);

		BYTE FinalR = RoundToInt(RV0 * (1.0f - s - t) + RV1 * s + RV2 * t);
		BYTE FinalG = RoundToInt(GV0 * (1.0f - s - t) + GV1 * s + GV2 * t);
		BYTE FinalB = RoundToInt(BV0 * (1.0f - s - t) + BV1 * s + BV2 * t);
		return RGB32(FinalR, FinalG, FinalB);
	}
};

