#pragma once
#include"Model.h"
#include"../IModelState.h"
#include"CreateResources/CreateResources.h"
#include<numbers>
#include"ImGuiManager.h"
#include"Camera.h"
class StateSphere :public IModelState
{
public:

	void Initialize(Vector4 pos, float size, WorldTransform worldTransform, texResourceProperty tex) override;

	void Draw()override;

	void Release()override;

	virtual void TransferMatrix(Matrix4x4 m)override;

	virtual void SetTexProperty(texResourceProperty NewTex)override;

	virtual Matrix4x4 GetWorldTransform()override;

private:

	void CommandCall();

	ResourcePeroperty resource_ = {};

	WorldTransform workdTransform_ = {};
	Vector4 color_ = {1,1,1,1};
	Vector4 centerPos_ = { 0,0,0,0 };
	float size_ = 0;
	texResourceProperty tex_ = {};
	const int VertexNum = 16;
	Vector3 directionPos = { 0.0f,-1.0f,0.0f };
};



