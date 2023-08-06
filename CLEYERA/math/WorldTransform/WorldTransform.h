#pragma once
#include"Matrix2x2.h"
#include"Matrix3x3.h"
#include"Matrix4x4.h"
#include"MatrixTransform.h"


#include"Vector2.h"
#include"Vector3.h"
#include"Vector4.h"



struct WorldTransform
{
	/// <summary>
	/// �傫��
	/// </summary>
	Vector3 scale_ = {1.0f,1.0f,1.0f};

	/// <summary>
	/// ��]
	/// </summary>
	Vector3 rotate_ = { 0.0f,0.0f,0.0f };

	/// <summary>
	/// ���s�ړ�
	/// </summary>
	Vector3 translation_ = { 0.0f,0.0f,0.0f };

	/// <summary>
	/// �s��
	/// </summary>
	Matrix4x4 matWorld = {0};

	void Initialize();

	void UpdateMatrix();

};