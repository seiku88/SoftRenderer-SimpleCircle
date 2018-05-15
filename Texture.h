#pragma once

#include "stdafx.h"
#include "Vector.h"

class Texture
{
public:
	Texture() : Buffer(NULL), width(0), height(0), isTransparent(false) {}
	~Texture() { if (Buffer != NULL) delete Buffer; }

	bool IsLoaded() { return (Buffer != NULL);  }

	bool LoadBMP(char* filename);
	void SetTransparent(ULONG col) { isTransparent = true; transparentColour = col; }
	bool IsColourTransparent(ULONG col)
	{
		if (isTransparent && col == transparentColour) return true;
		return false;
	}
	ULONG GetPixel(int x, int y);
	ULONG TextureSample(Vector2 TexCoord, float s, float t);

	int width;
	int height;		
	ULONG* Buffer;

	bool isTransparent;
	ULONG transparentColour;
};