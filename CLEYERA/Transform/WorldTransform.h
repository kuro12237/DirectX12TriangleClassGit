#pragma once
#include"Pch.h"
#include"MatrixTransform.h"
#include"CreateResource.h"
#include"ViewProjection.h"

struct WorldTransform
{
	Vector3 scale = {1.0f,1.0f,1.0f};
	Vector3 rotation = {0.0f,0.0f,0.0f};
	Vector3 translate = {0.0f,0.0f,0.0f};

	Matrix4x4 matWorld = {};

	void Initialize();

	void UpdateMatrix();

	void TransfarMatrix(ComPtr<ID3D12Resource>&wvpResource, ViewProjection *viewProjection);






};

