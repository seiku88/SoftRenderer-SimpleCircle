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

	void SetIdentity()
	{
		_11 = 1.0f;
		_12 = 0.0f;
		_21 = 0.0f;
		_22 = 1.0f;
	};

	void SetScale(float a, float b)
	{
		SetIdentity();
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

struct Matrix3
{
public:
	float _11, _12, _13;
	float _21, _22, _23;
	float _31, _32, _33;

	Matrix3() : _11(0), _12(0), _13(0), _21(0), _22(0), _23(0), _31(0), _32(0), _33(0) {};
	Matrix3(float n11, float n12, float n13, float n21, float n22, float n23, float n31, float n32, float n33)
	{
		_11 = n11;
		_12 = n12;
		_13 = n13;

		_21 = n21;
		_22 = n22;
		_23 = n23;

		_31 = n31;
		_32 = n32;
		_33 = n33;
	}

	Matrix3 operator *(const Matrix3 other) const;

	void SetIdentity()
	{
		_11 = 1.0f;
		_12 = 0.0f;
		_13 = 0.0f;
		_21 = 0.0f;
		_22 = 1.0f;
		_23 = 0.0f;
		_31 = 0.0f;
		_32 = 0.0f;
		_33 = 1.0f;
	};

	void SetTranslation(float x, float y)
	{
		SetIdentity();
		_13 = x;
		_23 = y;
	}

	void SetScale(float a, float b)
	{
		SetIdentity();
		_11 = a;
		_22 = b;
	}

	void SetRotation(float angle)
	{
		SetIdentity();
		_11 = cosf(Deg2Rad(angle));
		_12 = -sinf(Deg2Rad(angle));
		_21 = -_12;
		_22 = _11;
	}

	void SetReflect(float a, float b, float c)
	{
		_11 = -1.0f;
		_22 = -1.0f;
	}

	void Transpose()
	{
		float tmp = _12;
		_12 = _21;
		_21 = tmp;
		
		tmp = _13;
		_13 = _31;
		_31 = tmp;

		tmp = _23;
		_23 = _32;
		_32 = tmp;
	}
};