// stdafx.h : ���� ��������� ���� ��������� �ʴ�
// ǥ�� �ý��� ���� ���� �Ǵ� ������Ʈ ���� ���� ������
// ��� �ִ� ���� �����Դϴ�.
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // ���� ������ �ʴ� ������ Windows ������� �����մϴ�.
// Windows ��� ����:
#include <windows.h>

// C ��Ÿ�� ��� �����Դϴ�.
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <math.h>
#include <intrin.h>

// TODO: ���α׷��� �ʿ��� �߰� ����� ���⿡�� �����մϴ�.
#define FORCEINLINE __forceinline

#define PI 3.141592f
#define KINDA_SMALL_NUMBER 1.e-4f

#define RGBA32(r,g,b,a) ((ULONG)((BYTE)(b)) | (ULONG)(((BYTE)(g)) << 8) | (ULONG)(((BYTE)(r)) << 16) | (ULONG)(((BYTE)(a)) << 24))
#define RGB32(r,g,b) RGBA32(r, g, b, 255)

FORCEINLINE float Deg2Rad(float deg) { return (PI / 180.0f) * deg; }

FORCEINLINE float InvSqrt(float x)
{
	float xhalf = 0.5f*x;
	int i = *(int*)&x;
	i = 0x5f3759df - (i >> 1);
	x = *(float*)&i;
	x = x*(1.5f - xhalf*x*x);
	return x;
}

FORCEINLINE int RoundToInt(float f) { return _mm_cvt_ss2si(_mm_set_ss(f + f + 0.5f)) >> 1; } // Note: the x2 is to workaround the rounding-to-nearest-even-number issue when the fraction is .5