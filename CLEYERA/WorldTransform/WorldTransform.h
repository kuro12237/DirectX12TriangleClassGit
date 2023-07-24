#pragma once
#include"../Vector/Vector3.h"
#include"../Matrix/MatrixTransform.h"

struct WorldTransform
{



	void Initialize();
	


	Vector3 translate_;
	Vector3 rotate_;
	Vector3 scale_;

	Matrix4x4 m;


	MatrixTransform* matrixTransform=nullptr;

};


