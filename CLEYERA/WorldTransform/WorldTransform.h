#pragma once
#include"../Vector/Vector3.h"
#include"../Matrix/MatrixTransform.h"

struct WorldTransform
{

	Vector3 rotate_ = { 0,0,0 };

	Vector3 translate_= {0,0,0};

	Vector3 scale_ = { 1,1,1 };

	Matrix4x4 matWorld_;

	void Initiallize();

	void UpdateMatrix();

	MatrixTransform* matrixTransform = new MatrixTransform();

};