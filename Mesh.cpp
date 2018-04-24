#include "stdafx.h"
#include "SoftRenderer.h"
#include "GDIHelper.h"
#include "Mesh.h"
#include "Renderer.h"

void Mesh::MultiplyMatrix(const Matrix3 other)
{
	for (int i = 0; i < verticesSize; i++)
	{
		vertices[i].position = vertices[i].position*other;
	}
}

void Mesh::SetVertices(Vertex* verts, unsigned int size)
{
	if (vertices != NULL) delete[] vertices;
	vertices = NULL;

	vertices = verts;
	verticesSize = size;
}

void Mesh::SetIndexes(unsigned int* idxs, unsigned int size)
{
	if (indexes != NULL) delete[] indexes;
	indexes = NULL;

	indexes = idxs;
	indexesSize = size;
}

void Mesh::Draw()
{
	for (int i = 0; i < indexesSize; i+=3)
	{
		Triangle tri;
		tri.vertices[0] = vertices[indexes[i]];
		tri.vertices[1] = vertices[indexes[i+1]];
		tri.vertices[2] = vertices[indexes[i+2]];

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
					//v2f v;
					//ULONG FinalColour = FragmentShader(v);
					//g_CurrentColor = FinalColour;
					if (g_Texture->IsLoaded()) g_CurrentColor = g_Texture->GetTexturePixel(outS, outT, tri);
					else g_CurrentColor = tri.GetPixelColour(target, outS, outT);
					PutPixel(x, y);
				}
			}
		}
	}

	return;
}
