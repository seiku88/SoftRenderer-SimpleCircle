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
	void SetMatrix(Matrix3 t, Matrix3 s, Matrix3 r)
	{
		translation = t;
		scale = s;
		rot = r;
	}
};

