#pragma once
#include"WorldTransform.h"
#include"GraphicsPipeline.h"
#include"TexManager.h"
#include"State/Sphere/StateSphere.h"

#include"State/IModelState.h"

enum SlectModel
{
	Cube,
	Sphere,

};


class Model
{
public:
	Model();
	~Model();

	void Initialize(Vector4 pos, float size, WorldTransform worldTransform, texResourceProperty tex,SlectModel select);

	void Draw();

	void Release();
	
	void TransferMatrix(Matrix4x4 m);

	void SetTexProperty(texResourceProperty NewTex);

	Matrix4x4 GetWorldTransform() { return state_->GetWorldTransform(); }

private:

	
	Shaders shape_ = { nullptr,nullptr };

	IModelState* state_ = nullptr;

};



