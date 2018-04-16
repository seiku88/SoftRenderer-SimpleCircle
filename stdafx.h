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
#define PI 3.141592f
#define KINDA_SMALL_NUMBER 1.e-4f
#define FORCEINLINE __forceinline

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

FORCEINLINE int RoundToInt(float F)
{
	// Note: the x2 is to workaround the rounding-to-nearest-even-number issue when the fraction is .5
	return _mm_cvt_ss2si(_mm_set_ss(F + F + 0.5f)) >> 1;
}