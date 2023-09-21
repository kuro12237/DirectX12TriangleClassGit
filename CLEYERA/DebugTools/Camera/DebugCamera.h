#pragma once
#include"Pch.h"
#include"ViewProjection.h"
#include"DebugTools/Commands/ICommand.h"
#include"Input.h"
#include"MatrixTransform.h"
#include"DebugTools/DebugTools.h"

class DebugCamera : public ICommand
{
public:
	DebugCamera() {};
	~DebugCamera() {};

	static DebugCamera* GetInstance();

	//èâä˙âª
	void Initialize();


	void UpdateExecute()override;
	void DrawExecute(ViewProjection viewProjection)override;
private:

	static void IsUseDebugCameraViewProjection();
	static void TranslateMove();
	static void RotateMove();
	
	WorldTransform worldTransform_{};
	ViewProjection DebugViewProjection_{};
	ViewProjection CameraObjeViewProjection_{};

	bool isCamera = false;
	Vector3 CemeraMoveSpeed_ = {0.04f,0.04f,0.04f};
	Vector3 CemaraRotateSpeed_ = {0.04f,0.04f,0.04f};

	Vector3 offset_ = { 0.0f,0.0f,-30.0f };
	Matrix4x4 matRotate_{};
};


