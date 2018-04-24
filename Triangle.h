#pragma once

#include "stdafx.h"
#include "Vector.h"

struct Vertex
{
public:
	Vertex() {}
	Vector3 position;
	Vector2 uv;
	ULONG colour;
};

struct appdata_full
{
	Vector3 vertex;  // The vertex position in model space.
	Vector2 texcoord; // The first UV coordinate.
	ULONG color;     // Per-vertex color
};

struct v2f
{
	Vector3 vertex;  // The vertex position in model space.
	Vector2 texcoord; // The first UV coordinate.
	ULONG color;     // Per-vertex color
};

class Triangle
{
public:
	IntPoint minPos, maxPos;
	Vertex vertices[3];

	Vector2 u, v, w;
	float dotUU, dotUV, dotVV;
	float invDenom;

	Triangle() {}
	Triangle(Vertex vert1, Vertex vert2, Vertex vert3)
	{
		vertices[0] = vert1;
		vertices[1] = vert2;
		vertices[2] = vert3;

		Vector2 sbbMin = Vector2(INFINITY, INFINITY);
		Vector2 sbbMax = Vector2(-INFINITY, -INFINITY);

		for (int i = 0; i < 3; i++)
		{
			Vector3 t = vertices[i].position;
			if (t.x < sbbMin.x) sbbMin.x = t.x;
			if (t.y < sbbMin.y) sbbMin.y = t.y;
			if (t.x > sbbMax.x) sbbMax.x = t.x;
			if (t.y > sbbMax.y) sbbMax.y = t.y;
		}

		u = (vertices[1].position - vertices[0].position).ToVector2();
		v = (vertices[2].position - vertices[0].position).ToVector2();
		dotUU = Vector2::Dot(u, u);
		dotUV = Vector2::Dot(u, v);
		dotVV = Vector2::Dot(v, v);
		invDenom = 1.0f / (dotUU * dotVV - dotUV * dotUV);

		minPos.x = RoundToInt(sbbMin.x);
		minPos.y = RoundToInt(sbbMin.y);
		maxPos.x = RoundToInt(sbbMax.x);
		maxPos.y = RoundToInt(sbbMax.y);
	}

	Triangle operator *(const Matrix3 other);

	void CalculateMinMaxPos();

	void CalcBaryCentricCoord(Vector3 target, float *outS, float *outT)
	{
		Vector2 w = (target - vertices[0].position).ToVector2();
		float dotUW = Vector2::Dot(u, w);
		float dotVW = Vector2::Dot(v, w);
		*outS = (dotVV * dotUW - dotUV * dotVW) * invDenom;
		*outT = (dotUU * dotVW - dotUV * dotUW) * invDenom;
	}

	bool IsInArea(IntPoint pos, float s, float t)
	{
		if (s >= 0.0f)
		{
			if (t >= 0.0f)
			{
				if (s + t <= 1.0f) return true;
			}
		}

		return false;
	}

	ULONG GetPixelColour(Vector3 target, float s, float t)
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
		return RGB(FinalB, FinalG, FinalR);
	}

	ULONG GetTextureColour(float s, float t, const Triangle& tri)
	{
		Vector2 UV0 = tri.vertices[0].uv;
		Vector2 UV0ToUV1 = tri.vertices[1].uv - tri.vertices[0].uv;
		Vector2 UV0ToUV2 = tri.vertices[2].uv - tri.vertices[0].uv;

		Vector2 UVResult = UV0 + UV0ToUV1*s + UV0ToUV2*t;

		//int pixX = UVResult.x - (Width - 1);
		//int pixY = UVResult.y - (Height - 1);
		return 0;
	}
};

