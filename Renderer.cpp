
#include "stdafx.h"
#include "Mesh.h"
#include "SoftRenderer.h"
#include "GDIHelper.h"
#include "Renderer.h"

#include <algorithm>
#include <vector>

bool IsInRange(int x, int y)
{
	return (abs(x) < (g_nClientWidth / 2)) && (abs(y) < (g_nClientHeight / 2));
}

void PutPixel(IntPoint point)
{
	if (!IsInRange(point.x, point.y)) return;

	ULONG* dest = (ULONG*)g_pBits;

	int halfWidth = RoundToInt(g_nClientWidth * 0.5f);
	int halfHeight = RoundToInt(g_nClientHeight * 0.5f);

	DWORD offset = (halfHeight * g_nClientWidth - g_nClientWidth * point.y) + (halfWidth + point.x);
	*(dest + offset) = g_CurrentColor;
}

void DrawCall(Mesh* MeshToDraw)
{
	v2f* v = new v2f[MeshToDraw->verticesSize];

	for (int i = 0; i < MeshToDraw->verticesSize; i++)
	{
		Vertex vert = MeshToDraw->vertices[i];
		appdata_full vdata;
		vdata.vertex = vert.position;
		vdata.colour = vert.colour;
		vdata.texcoord = vert.uv;
		v[i] = VertexShader(vdata, MeshToDraw->translation * MeshToDraw->rot * MeshToDraw->scale);
	}

	for (int i = 0; i < MeshToDraw->indexesSize; i+=3)
	{
		Triangle t(v[MeshToDraw->indexes[i]], v[MeshToDraw->indexes[i+1]], v[MeshToDraw->indexes[i+2]]);

		// Rasterization
		for (int y = t.minPos.y; y <= t.maxPos.y; y++)
		{
			for (int x = t.minPos.x; x <= t.maxPos.x; x++)
			{
				Vector3 target((float)x + 0.5f, (float)y + 0.5f, 0.0f);
				float outS, outT;
				t.CalcBaryCentricCoord(target, &outS, &outT);
				if (t.IsInArea(outS, outT))
				{
					v2f v2f;
					v2f.vertex = t.GetFragmentPos(target, outS, outT);
					v2f.colour = t.GetFragmentColor(target, outS, outT);
					v2f.texcoord = t.GetFragmentUV(target, outS, outT);
					ULONG finalColor = FragmentShader(v2f, outS, outT);
					if (g_Texture->isTransparent)
					{
						if (g_Texture->IsColourTransparent(finalColor)) continue;
					}
					g_CurrentColor = finalColor;
					PutPixel(IntPoint(x, y));
				}
			}
		}
	}

	if (NULL != v)
	{
		delete[] v;
	}
}

//Shader
v2f VertexShader(appdata_full in, Matrix3 matcmb)
{
	// TRS Matrix 반영해서 변환된 position 돌려주기..
	return v2f(in.vertex*matcmb, in.texcoord, in.colour);
}

ULONG FragmentShader(v2f in, float s, float t)
{
	// Texture에서 색상 빼오기..
	if (g_Texture->IsLoaded())
	{
		return g_Texture->TextureSample(in.texcoord, s, t);
	}

	return in.colour;
}

//Primitives
void DrawEllipse(Vector2 pivot, Matrix2 mat, int size)
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
				PutPixel(IntPoint(newPos.x, newPos.y));
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
		PutPixel(IntPoint(((1 - t)*a.x + t*b.x), ((1 - t)*a.y + t*b.y)));
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
				PutPixel(IntPoint(position.x + x, position.y + y));
			}
		}
	}

	SetColor(255, 0, 0);
	PutPixel(IntPoint(position.x, position.y));
}

bool SortMeshByLayer(const Mesh* lhs, const Mesh* rhs) { return lhs->layer > rhs->layer; } //Smaller number first

void InitFrame(void)
{
}

void UpdateFrame(void)
{
	// Buffer Clear
	SetColor(32, 32, 32);
	Clear();

	//Inputs
	static Vector3 pos;
	static float angle = 0.0f;
	static float dist = 0.0f;

	//if (GetAsyncKeyState(VK_LEFT)) dist -= 10.0f;
	//if (GetAsyncKeyState(VK_RIGHT)) dist += 10.0f;
	//if (GetAsyncKeyState(VK_UP)) pos.y += 1.0f;
	//if (GetAsyncKeyState(VK_DOWN)) pos.y -= 1.0f;
	//angle += 0.1f;

	//Shapes
	SetColor(255, 255, 255);

	//Matrix
	Matrix3 translation, scale, rot;

	Vector3 pivot(0, 0, 1); //Pivot
	Vector3 startvec, endvec;
	startvec.SetType(g_nMousePositionX - (g_nClientWidth / 2), -(g_nMousePositionY - (g_nClientHeight / 2)), true);
	endvec.SetType(g_nMouseSubPositionX - (g_nClientWidth / 2), -(g_nMouseSubPositionY - (g_nClientHeight / 2)), true);

	//Meshes
	std::vector<Mesh*> meshes;
	int indexes[6] = { 0,1,2,1,2,3 }; //Default indexes for rectangle mesh

	Mesh* skymesh = new Mesh();
	Vertex vertssky[4];
	vertssky[0].position = Vector3(-100, 100, 0);
	vertssky[1].position = Vector3(100, 100, 0);
	vertssky[2].position = Vector3(-100, -100, 0);
	vertssky[3].position = Vector3(100, -100, 0);
	vertssky[0].uv = Vector2(0, 0.5f);
	vertssky[1].uv = Vector2(1, 0.5f);
	vertssky[2].uv = Vector2(0, 1);
	vertssky[3].uv = Vector2(1, 1);
	translation.SetTranslation(dist/2, pos.y);
	scale.SetScale(1.9f, 1);
	rot.SetRotation(0);
	skymesh->SetMatrix(translation, scale, rot);
	skymesh->SetVertices(vertssky, 4);
	skymesh->SetIndexes(indexes, 6);
	skymesh->layer = 4;
	meshes.push_back(skymesh);

	Mesh* groundmesh = new Mesh();
	Vertex vertsground[4];
	vertsground[0].position = Vector3(-100, 100, 0);
	vertsground[1].position = Vector3(100, 100, 0);
	vertsground[2].position = Vector3(-100, -100, 0);
	vertsground[3].position = Vector3(100, -100, 0);
	vertsground[0].uv = Vector2(0.5f, 0);
	vertsground[1].uv = Vector2(1, 0);
	vertsground[2].uv = Vector2(0.5f, 0.5f);
	vertsground[3].uv = Vector2(1, 0.5f);
	translation.SetTranslation(dist / 1.5f, pos.y);
	scale.SetScale(2, 1);
	groundmesh->SetMatrix(translation, scale, rot);
	groundmesh->SetVertices(vertsground, 4);
	groundmesh->SetIndexes(indexes, 6);
	groundmesh->layer = 2;
	meshes.push_back(groundmesh);

	Mesh* charamesh = new Mesh();
	Vertex vertschar[4];
	vertschar[0].position = Vector3(-100 + startvec.x, 100 + startvec.y, 0);
	vertschar[1].position = Vector3(100 + startvec.x, 100 + startvec.y, 0);
	vertschar[2].position = Vector3(-100 + startvec.x, -100 + startvec.y, 0);
	vertschar[3].position = Vector3(100 + startvec.x, -100 + startvec.y, 0);
	vertschar[0].uv = Vector2(0, 0);
	vertschar[1].uv = Vector2(0.5f, 0);
	vertschar[2].uv = Vector2(0, 0.5f);
	vertschar[3].uv = Vector2(0.5f, 0.5f);
	translation.SetTranslation(dist, pos.y);
	scale.SetScale(1, 1);
	rot.SetRotation(g_nMouseWheel / 10 + angle);
	charamesh->SetMatrix(translation, scale, rot);
	charamesh->SetVertices(vertschar, 4);
	charamesh->SetIndexes(indexes, 6);
	charamesh->layer = 0;
	meshes.push_back(charamesh);

	//Sort meshes by layer
	std::sort(meshes.begin(), meshes.end(), SortMeshByLayer);

	//Draw all meshes
	for (int i = 0; i < meshes.size(); i++)//Use iterator? (std::vector<Mesh*>::iterator it = meshes.begin(); it != meshes.end(); ++it)
	{
		DrawCall(meshes[i]);
	}
	meshes.clear();

	// Buffer Swap 
	BufferSwap();
}
