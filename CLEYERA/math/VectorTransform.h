#pragma once
#include"Pch.h"

class VectorTransform
{
public:
	
	static Vector3 Add(const Vector3 &v1, const Vector3 &v2);

	static Vector3 TransformNormal(const Vector3& v, const Matrix4x4& m);
private:

};

