
#include "stdafx.h"
#include "Vector.h"
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


void UpdateFrame(void)
{
	// Buffer Clear
	SetColor(32, 128, 255);
	Clear();

	// Draw
	SetColor(255, 0, 0);
	PutPixel(0, 0);

	//Shapes
	SetColor(255, 255, 255);

	Vector2 v(g_nMousePositionX - (g_nClientWidth / 2), -(g_nMousePositionY - (g_nClientHeight / 2))); //Pivot
	Matrix2 scale;
	scale.SetScale(1.0f, 2.5f);
	scale.SetShear(0.0f, 0.0f);

	static float angle = 0.0f;
	Matrix2 rot;
	rot.SetRotation(angle);
	angle += 0.5f;

	float size = 10 + (g_nMouseWheel/10); //Radius

	//x^2 + y^2 = r^2
	for (int w = v.x-size; w < v.x+size; w++)
	{
		for (int h = v.y-size; h < v.y+size; h++)
		{
			Vector2 curPos(w, h);
			if (Vector2::DistNonSquared(v, curPos) < size * size)//if (((w-v.x)*(w-v.x)) + ((h-v.y)*(h-v.y)) < size*size)
			{
				Vector2 newPos(curPos*scale);
				newPos = newPos*rot;
				PutPixel(newPos.x, newPos.y);
			}
		}
	}
	
	SetColor(255, 0, 0);
	PutPixel(v.x, v.y); //Circle's Pivot

	/* //Using Sine and Cosine
	for (int i = 0; i < 360; i++)
	{
		PutPixel(cos(i) * size, sin(i) * size);
	}*/

	// Buffer Swap 
	BufferSwap();
}
