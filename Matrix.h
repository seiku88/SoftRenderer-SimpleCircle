#pragma once

#include "stdafx.h"

struct Matrix2
{
public:
	float _11, _12;
	float _21, _22;

	Matrix2() {};
	Matrix2(float n11, float n12, float n21, float n22)
	{
		_11 = n11;
		_12 = n12;

		_21 = n21;
		_22 = n22;
	}

	Matrix2 operator *(const Matrix2 other) const;

	void SetScale(float a, float b)
	{
		_11 = a;
		_22 = b;
	}

	void SetShear(float a, float b)
	{
		_12 = a;
		_21 = b;
	}

	void SetRotation(float angle)
	{
		float rad = Deg2Rad(angle);
		_11 = cosf(rad);
		_12 = -sinf(rad);
		_21 = sinf(rad);
		_22 = cosf(rad);
	}
};