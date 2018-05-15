#include "stdafx.h"
#include "Mesh.h"

void Mesh::SetVertices(Vertex* verts, size_t size)
{
	if (vertices != NULL) delete[] vertices;
	vertices = NULL;

	vertices = verts;
	verticesSize = size;
}

void Mesh::SetIndexes(int* idxs, size_t size)
{
	if (indexes != NULL) delete[] indexes;
	indexes = NULL;

	indexes = idxs;
	indexesSize = size;
}