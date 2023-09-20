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

	static void Initialize();

	static ViewProjection DebugCameraViewProjection();

	void execute(ViewProjection viewProjection)override;
private:


	ViewProjection DebugViewProjection_{};
	ViewProjection CameraObjeViewProjection_{};

	bool isCamera = false;

	Vector3 CemeraMoveSpeed_ = {};
	Vector3 CemaraRotateSpeed_ = {};
	float CameraZoom_ = {};

};


