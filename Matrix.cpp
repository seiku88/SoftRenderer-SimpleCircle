
#include "stdafx.h"
#include "Matrix.h"

Matrix2 Matrix2::operator *(const Matrix2 other) const
{
	Matrix2 result;

	result._11 = _11*other._11 + _12*other._21;
	result._12 = _12*other._12 + _12*other._22;
	result._21 = _21*other._11 + _22*other._21;
	result._22 = _21*other._12 + _22*other._22;

	return result;
}