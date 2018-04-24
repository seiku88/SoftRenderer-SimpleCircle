
#include "stdafx.h"
#include "Mesh.h"
#include "SoftRenderer.h"
#include "GDIHelper.h"
#include "Renderer.h"

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
	float outS, outT;
	for (int y = RoundToInt(tri.minPos.y); y < RoundToInt(tri.maxPos.y); y++)
	{
		for (int x = RoundToInt(tri.minPos.x); x < RoundToInt(tri.maxPos.x); x++)
		{
			Vector3 target((float)x, (float)y, 0.0f);
			tri.CalcBaryCentricCoord(target, &outS, &outT);
			if (tri.IsInArea(IntPoint(x, y), outS, outT))
			{
				if (g_Texture->IsLoaded()) g_CurrentColor = g_Texture->GetTexturePixel(outS, outT, tri);
				else g_CurrentColor = tri.GetPixelColour(target, outS, outT);
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
	angle += 1.0f;

	//Shapes
	SetColor(255, 255, 255);

	Vector3 pivot(0, 0, 1); //Pivot
	Vector3 startvec, endvec;
	startvec.SetType(g_nMousePositionX - (g_nClientWidth / 2), -(g_nMousePositionY - (g_nClientHeight / 2)), true);
	endvec.SetType(g_nMouseSubPositionX - (g_nClientWidth / 2), -(g_nMouseSubPositionY - (g_nClientHeight / 2)), true);

	Mesh* rectmesh = new Mesh();
	Vertex verts[4];
	verts[0].position = Vector3(-100, 100, 0);
	verts[0].uv = Vector2(0, 0);
	verts[1].position = Vector3(100, 100, 0);
	verts[1].uv = Vector2(1, 0);
	verts[2].position = Vector3(-100, -100, 0);
	verts[2].uv = Vector2(0, 1);
	verts[3].position = Vector3(100, -100, 0);
	verts[3].uv = Vector2(1, 1);

	unsigned int indexes[6];
	indexes[0] = 0;
	indexes[1] = 1;
	indexes[2] = 2;
	indexes[3] = 1;
	indexes[4] = 2;
	indexes[5] = 3;

	rectmesh->SetVertices(verts, 4);
	rectmesh->SetIndexes(indexes, 6);

	Triangle tri;
	tri.vertices[0].position = pivot;
	tri.vertices[1].position = startvec;
	tri.vertices[2].position = endvec;

	Matrix3 translation, scale, rot;
	translation.SetTranslation(pos.x, pos.y);
	scale.SetScale(1, 1);
	rot.SetRotation(g_nMouseWheel / 10 + angle);

	Matrix3 matcmb = translation * rot * scale;

	tri = tri*matcmb;
	rectmesh->MultiplyMatrix(matcmb);

	tri.vertices[0].colour = RGB(219, 94, 77);
	tri.vertices[0].uv = Vector2(0, 0);
	tri.vertices[1].colour = RGB(140, 213, 94);
	tri.vertices[1].uv = Vector2(1, 0);
	tri.vertices[2].colour = RGB(74, 104, 198);
	tri.vertices[2].uv = Vector2(1, 1);

	//DrawTriangle(tri);
	rectmesh->Draw();

	SetColor(255, 0, 0);
	DrawLine(startvec, endvec);
	DrawCrosshair(startvec.ToVector2()); //Mouse Click Position
	DrawCrosshair(endvec.ToVector2()); //Right Mouse Click Position

	//delete rectmesh;

	// Buffer Swap 
	BufferSwap();
}
