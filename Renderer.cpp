
#include "stdafx.h"
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
	int x = 100, y = -20; //Pivot
	int size = 100; //Radius

	//x^2 + y^2 = r^2
	for (int w = x-size; w < x+size; w++)
	{
		for (int h = y-size; h < y+size; h++)
		{
			if (((w-x)*(w-x)) + ((h-y)*(h-y)) < size*size)
			{
				PutPixel(w, h);
			}
		}
	}
	
	SetColor(255, 0, 0);
	PutPixel(x, y); //Circle's Pivot

	/* //Using Sine and Cosine
	for (int i = 0; i < 360; i++)
	{
		PutPixel(cos(i) * size, sin(i) * size);
	}*/

	// Buffer Swap 
	BufferSwap();
}
