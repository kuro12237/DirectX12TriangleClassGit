#pragma once
#include"Pch.h"
#include"ModelPlaneState.h"
#include"ModelLineState.h"

#include"WorldTransform.h"
#include"ViewProjection.h"
class Model
{
public:
	~Model();
	/// <summary>
	/// モデルの初期化
	/// </summary>
	/// <param name="モード選択"></param>
	void Initialize(IModelState* state, Vector4 CenterPos = {0,0,0,1}, float size = { 0.5 }, Vector4 color = {1,1,1,1});
	
	/// <summary>
	/// DrawLineの初期化
	/// </summary>
	/// <param name="StertPosition"></param>
	/// <param name="EndPosition"></param>
	/// 
	void Initialize(IModelState* state, Vector4 StertPosition, Vector4 EndPosition, Vector4 Color = { 1,1,1,1 });
	
	void Draw(WorldTransform worldTransform, ViewProjection viewprojection);

	WorldTransform GetWorldTransform() { return worldTransform_; }
	Vector4 GetColor() { return color_; }
	
	Vector4 GetStartPos() { return StartPos_;}
	Vector4 GetEndPos(){ return EndPos_; }
	
	
	Vector4 GetCenterPos(){ return CenterPos_; }
	float GetSize(){ return size_; }



private:

	
	Vector4 CenterPos_ = { 0,0,0,1 };
	float size_=0.5f;

	Vector4 StartPos_ = {};
	Vector4 EndPos_ = {};

	Vector4 color_ = { 1,0,0,1 };
	WorldTransform worldTransform_ = {};

	IModelState* state_ = nullptr;
};

