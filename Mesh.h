#pragma once

#include "stdafx.h"
#include "Vertex.h"

class Mesh
{
public:
	Vertex* vertices;
	size_t verticesSize;

	int* indexes;
	size_t indexesSize;

	Matrix3 translation, scale, rot;
	int layer;


	Mesh()
	{
		if (vertices != NULL) delete[] vertices;
		if (indexes != NULL) delete[] indexes;

		vertices = 0;
		indexes = 0;

		layer = 0;
	}

	Mesh::~Mesh()
	{
		if (vertices != NULL) delete[] vertices;
		if (indexes != NULL) delete[] indexes;

		vertices = 0;
		indexes = 0;
	}

	void SetVertices(Vertex* verts, size_t size);
	void SetIndexes(int* idxs, size_t size);
	void SetMatrix(Vector2 t, Vector2 s, float r)
	{
		translation.SetTranslation(t.x, t.y);
		scale.SetScale(s.x, s.y);
		rot.SetRotation(r);
	}
};

