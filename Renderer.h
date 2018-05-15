#pragma once
#include "Mesh.h"
#include "Triangle.h"

void InitFrame(void);
void UpdateFrame(void);

bool IsInRange(int x, int y);
void PutPixel(IntPoint point);
void DrawCall(Mesh* MeshToDraw);

v2f VertexShader(appdata_full in, Matrix3 matcmb);
ULONG FragmentShader(v2f in, float s, float t);