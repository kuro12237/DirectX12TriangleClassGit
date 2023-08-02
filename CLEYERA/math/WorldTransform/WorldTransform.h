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
	/// 大きさ
	/// </summary>
	Vector3 scale_ = {1.0f,1.0f,1.0f};

	/// <summary>
	/// 回転
	/// </summary>
	Vector3 rotate_ = { 0.0f,0.0f,0.0f };

	/// <summary>
	/// 平行移動
	/// </summary>
	Vector3 translation_ = { 0.0f,0.0f,0.0f };

	/// <summary>
	/// 行列
	/// </summary>
	Matrix4x4 matWorld;

	void Initialize();


};