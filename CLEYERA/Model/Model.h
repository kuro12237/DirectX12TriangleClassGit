#pragma once
#include"Pch.h"
#include"ModelPlaneState.h"
#include"WorldTransform.h"
#include"ViewProjection.h"
class Model
{
public:
	~Model();
	void Initialize(IModelState *state);
	void Draw(WorldTransform worldTransform);

	WorldTransform GetWorldTransform() { return worldTransform_; }
	Vector4 GetColor() { return color_; }
	Vector4 GetCenterPos(){ return CenterPos_; }
	float GetSize(){ return size_; }

private:

	WorldTransform worldTransform_ = {};
	Vector4 color_ = { 1,0,0,1 };
	Vector4 CenterPos_ = { 0,0,0,1 };
	float size_=0.5f;

    IModelState* state_ = nullptr;
};

