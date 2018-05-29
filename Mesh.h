#pragma once

#include "stdafx.h"
#include "Transform.h"

class Mesh
{
public:
	Vertex* vertices;
	size_t verticesSize;

	int* indexes;
	size_t indexesSize;

	Transform2D transform;
	int layer;


	Mesh()
	{
		if (vertices != NULL) delete[] vertices;
		if (indexes != NULL) delete[] indexes;

		vertices = 0;
		indexes = 0;

		transform = Transform2D();
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
	void SetTransform(Vector2 t, Vector2 s, float r)
	{
		transform.position = t;
		transform.scale = s;
		transform.rotation = r;
	}
};

