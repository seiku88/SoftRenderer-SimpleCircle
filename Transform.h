#include "Vertex.h"

class Transform2D
{
public:
	Vector2 position;
	Vector2 scale;
	float rotation;

	Transform2D() : position(Vector2()), scale(Vector2()), rotation(0) {}
	Transform2D(Vector2 p, Vector2 s, float r) : position(p), scale(s), rotation(r) {}
	~Transform2D() {}

	Matrix3 GetCombinedMatrix()
	{
		Matrix3 TMat, RMat, SMat;
		TMat.SetTranslation(position.x, position.y);
		RMat.SetRotation(rotation);
		SMat.SetScale(scale.x, scale.y);
		return TMat * RMat * SMat;
	}

	Matrix3 GetViewMatrix()
	{
		Matrix3 TMat, RMat;
		TMat.SetTranslation(-position.x, -position.y);
		RMat.SetRotation(rotation);
		RMat.Transpose();
		return RMat * TMat;
	}
};

class Transform
{
public:
	Vector3 position;
	Vector3 scale;
	Vector3 rotation;

	Transform() {}
	~Transform() {}
};