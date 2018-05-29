
#include "stdafx.h"
#include "Mesh.h"
#include "SoftRenderer.h"
#include "GDIHelper.h"
#include "Renderer.h"

#include <algorithm>
#include <vector>

//Scene Variables
Matrix3 g_ShaderMatrix;

Vector2 pos;
float angle = 0.0f;
Vector2 scale;

std::vector<Mesh *> meshes;
Mesh *skymesh, *groundmesh, *charamesh;
Vertex vertssky[4], vertsground[4], vertschar[4];
int indexes[6] = { 0,1,2,1,2,3 }; //Default indexes for rectangle mesh

//Rendering Functions
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
		v[i] = VertexShader(vdata, g_ShaderMatrix * MeshToDraw->transform.GetCombinedMatrix());
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
	v2f result;
	result.vertex = in.vertex*matcmb;
	result.texcoord = in.texcoord;
	result.colour = in.colour;
	return result;
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
	IntPoint tmp;
	for (int i = 0; i < dist; i++)
	{
		float t = i / dist;
		tmp.x = (1 - t)*a.x + t * b.x;
		tmp.y = (1 - t)*a.y + t * b.y;
		if (tmp.x >= -0.5f * g_nClientWidth && tmp.x <= 0.5f * g_nClientWidth && tmp.y >= -0.5f * g_nClientHeight && tmp.y <= 0.5f * g_nClientHeight) PutPixel(tmp);
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
	pos = Vector2(0, 0);
	scale = Vector2(1, 1);

	skymesh = new Mesh();
	groundmesh = new Mesh();
	charamesh = new Mesh();

	vertssky[0].position = Vector3(-100, 100);
	vertssky[1].position = Vector3(100, 100);
	vertssky[2].position = Vector3(-100, -100);
	vertssky[3].position = Vector3(100, -100);
	vertssky[0].uv = Vector2(0, 0.5f);
	vertssky[1].uv = Vector2(1, 0.5f);
	vertssky[2].uv = Vector2(0, 1);
	vertssky[3].uv = Vector2(1, 1);
	skymesh->SetTransform(Vector2(pos.x / 2, pos.y), Vector2(1.9f, 1), 0);
	skymesh->SetVertices(vertssky, 4);
	skymesh->SetIndexes(indexes, 6);
	skymesh->layer = 4;

	vertsground[0].position = Vector3(-100, 100);
	vertsground[1].position = Vector3(100, 100);
	vertsground[2].position = Vector3(-100, -100);
	vertsground[3].position = Vector3(100, -100);
	vertsground[0].uv = Vector2(0.5f, 0);
	vertsground[1].uv = Vector2(1, 0);
	vertsground[2].uv = Vector2(0.5f, 0.5f);
	vertsground[3].uv = Vector2(1, 0.5f);
	groundmesh->SetTransform(Vector2(pos.x / 1.5f, pos.y), Vector2(2, 1), 0);
	groundmesh->SetVertices(vertsground, 4);
	groundmesh->SetIndexes(indexes, 6);
	groundmesh->layer = 2;

	vertschar[0].position = Vector3(-100, 100);
	vertschar[1].position = Vector3(100, 100);
	vertschar[2].position = Vector3(-100, -100);
	vertschar[3].position = Vector3(100, -100);
	vertschar[0].uv = Vector2(0, 0);
	vertschar[1].uv = Vector2(0.5f, 0);
	vertschar[2].uv = Vector2(0, 0.5f);
	vertschar[3].uv = Vector2(0.5f, 0.5f);
	charamesh->SetTransform(Vector2(pos.x, pos.y), Vector2(1, 1), g_nMouseWheel / 10 + angle);
	charamesh->SetVertices(vertschar, 4);
	charamesh->SetIndexes(indexes, 6);
	charamesh->layer = 0;

	//Push all meshes
	//meshes.push_back(groundmesh);
	meshes.push_back(charamesh);
	//meshes.push_back(skymesh);
}

void UpdateFrame(void)
{
	// Buffer Clear
	SetColor(32, 32, 32);
	Clear();

	//Inputs
	static Vector2 camLocation(0.0f, 0.0f);
	static float camRotation = 0.0f;

	if (GetAsyncKeyState(VK_LSHIFT))
	{
		if (GetAsyncKeyState(VK_LEFT)) camLocation.x -= 1.0f;
		if (GetAsyncKeyState(VK_RIGHT)) camLocation.x += 1.0f;
		if (GetAsyncKeyState(VK_UP)) camLocation.y += 1.0f;
		if (GetAsyncKeyState(VK_DOWN)) camLocation.y -= 1.0f;
		if (GetAsyncKeyState(VK_PRIOR)) camRotation += 1.0f;
		if (GetAsyncKeyState(VK_NEXT)) camRotation -= 1.0f;
	}
	else
	{
		if (GetAsyncKeyState(VK_LEFT)) pos.x -= 1.0f;
		if (GetAsyncKeyState(VK_RIGHT)) pos.x += 1.0f;
		if (GetAsyncKeyState(VK_UP)) angle += 1.0f;
		if (GetAsyncKeyState(VK_DOWN)) angle -= 1.0f;
		if (GetAsyncKeyState(VK_PRIOR)) scale *= 1.01f;
		if (GetAsyncKeyState(VK_NEXT)) scale *= 0.99f;
	}
	angle += 0.5f;

	Vector3 pivot(0, 0, 1); //Pivot
	Vector3 startvec, endvec;
	startvec.SetType(g_nMousePositionX - (g_nClientWidth / 2), -(g_nMousePositionY - (g_nClientHeight / 2)), true);
	endvec.SetType(g_nMouseSubPositionX - (g_nClientWidth / 2), -(g_nMouseSubPositionY - (g_nClientHeight / 2)), true);

	//====Mesh Changes
	//skymesh->SetTransform(Vector2(pos.x / 2, pos.y), Vector2(1.9f, 1), 0);
	//groundmesh->SetTransform(Vector2(pos.x / 1.5f, pos.y), Vector2(2, 1), 0);
	charamesh->SetTransform(pos, scale, g_nMouseWheel / 10 + angle);

	//Sort meshes by layer
	std::sort(meshes.begin(), meshes.end(), SortMeshByLayer);


	//====Camera
	Transform2D camera(camLocation, Vector2(1, 1), camRotation);
	Matrix3 ViewMat = camera.GetViewMatrix();

	g_ShaderMatrix = ViewMat;


	//====Drawing
	//Axis Draw
	Vector3 XStart((float)g_nClientWidth * -0.7f + camLocation.x, 0.0f, 1.0f);
	Vector3 XEnd((float)g_nClientWidth * 0.7f + camLocation.x, 0.0f, 1.0f);
	SetColor(255, 0, 0);
	DrawLine(XStart * ViewMat, XEnd * ViewMat);

	Vector3 YStart(0.0f, (float)g_nClientHeight * -0.7f + camLocation.y, 1.0f);
	Vector3 YEnd(0.0f, (float)g_nClientHeight * 0.7f + camLocation.y, 1.0f);
	SetColor(0, 255, 0);
	DrawLine(YStart * ViewMat, YEnd * ViewMat);

	//Draw all meshes
	for (std::vector<Mesh*>::iterator it = meshes.begin(); it != meshes.end(); ++it)
	{
		DrawCall(*it);
	}


	//Buffer Swap 
	BufferSwap();
}
