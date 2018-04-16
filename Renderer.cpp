
#include "stdafx.h"
#include "Triangle.h"
#include "SoftRenderer.h"
#include "GDIHelper.h"
#include "Renderer.h"

bool IsInRange(int x, int y);
void PutPixel(int x, int y);

bool IsInRange(int x, int y)
{
	return (abs(x) < (g_nClientWidth / 2)) && (abs(y) < (g_nClientHeight / 2));
}

void PutPixel(int x, int y)
{
	if (!IsInRange(x, y)) return;

	ULONG* dest = (ULONG*)g_pBits;
	DWORD offset = g_nClientWidth * g_nClientHeight / 2 + g_nClientWidth / 2 + x + g_nClientWidth * -y;
	*(dest + offset) = g_CurrentColor;
}

void PutPixel(IntPoint point)
{
	if (!IsInRange(point.x, point.y)) return;

	ULONG* dest = (ULONG*)g_pBits;
	DWORD offset = g_nClientWidth * g_nClientHeight / 2 + g_nClientWidth / 2 + point.x + g_nClientWidth * -point.y;
	*(dest + offset) = g_CurrentColor;
}

void DrawEllipse(Vector2 pivot, Matrix2 mat, float size)
{
	//x^2 + y^2 = r^2
	for (int w = pivot.x - size; w < pivot.x + size; w++)
	{
		for (int h = pivot.y - size; h < pivot.y + size; h++)
		{
			Vector2 curPos(w, h);
			if (Vector2::DistNonSquared(pivot, curPos) < size * size)//if (((w-v.x)*(w-v.x)) + ((h-v.y)*(h-v.y)) < size*size)
			{
				Vector2 newPos(curPos*mat);
				PutPixel(newPos.x, newPos.y);
			}
		}
	}
}

void DrawLine(Vector3 a, Vector3 b)
{
	float dist = Vector3::Dist(a, b);
	for (int i = 0; i < dist; i++)
	{
		float t = i / dist;
		PutPixel(((1 - t)*a.x + t*b.x), ((1 - t)*a.y + t*b.y));
	}
}

void DrawTriangle(Triangle& tri)
{
	tri.CalculateMinMaxPos();
	for (int y = RoundToInt(tri.minPos.y); y < RoundToInt(tri.maxPos.y); y++)
	{
		for (int x = RoundToInt(tri.minPos.x); x < RoundToInt(tri.maxPos.x); x++)
		{
			if (tri.IsInArea(IntPoint(x, y)))
			{
				//ULONG col = tri.GetPointColour(IntPoint(x, y));
				//SetColor(GetRValue(col), GetGValue(col), GetBValue(col));
				PutPixel(IntPoint(x, y));
			}
		}
	}
}

void DrawCrosshair(const Vector2 position)
{
	SetColor(255, 255, 255);
	for (int x = -5; x <= 5; x++)
	{
		for (int y = -5; y <= 5; y++)
		{
			if (x == 0 || y == 0)
			{
				PutPixel(position.x + x, position.y + y);
			}
		}
	}

	SetColor(255, 0, 0);
	PutPixel(position.x, position.y);
}


void UpdateFrame(void)
{
	// Buffer Clear
	SetColor(32, 32, 32);
	Clear();

	// Draw
	SetColor(255, 0, 0);
	PutPixel(0, 0);

	//Inputs
	static Vector3 pos;
	static float angle = 0.0f;

	if (GetAsyncKeyState(VK_LEFT)) pos.x -= 1.0f;
	if (GetAsyncKeyState(VK_RIGHT)) pos.x += 1.0f;
	if (GetAsyncKeyState(VK_UP)) pos.y += 1.0f;
	if (GetAsyncKeyState(VK_DOWN)) pos.y -= 1.0f;

	//Shapes
	SetColor(255, 255, 255);

	Vector3 pivot(0, 0, 1); //Pivot
	Vector3 startvec, endvec;
	startvec.SetType(g_nMousePositionX - (g_nClientWidth / 2), -(g_nMousePositionY - (g_nClientHeight / 2)), true);
	endvec.SetType(g_nMouseSubPositionX - (g_nClientWidth / 2), -(g_nMouseSubPositionY - (g_nClientHeight / 2)), true);

	Triangle tri;
	tri.vertices[0].position = pivot;
	tri.vertices[1].position = startvec;
	tri.vertices[2].position = endvec;

	tri.vertices[0].colour = RGB(255, 0, 0);
	tri.vertices[1].colour = RGB(0, 255, 0);
	tri.vertices[2].colour = RGB(0, 0, 255);

	Matrix3 translation, scale, rot;
	translation.SetTranslation(pos.x, pos.y);
	scale.SetScale(1, 1);
	rot.SetRotation(g_nMouseWheel / 10);

	Matrix3 matcmb = translation * rot * scale;

	tri = tri*matcmb;

	DrawTriangle(tri);

	SetColor(255, 0, 0);
	DrawLine(startvec, endvec);
	DrawCrosshair(startvec.ToVector2()); //Mouse Click Position
	DrawCrosshair(endvec.ToVector2()); //Right Mouse Click Position

	// Buffer Swap 
	BufferSwap();
}
