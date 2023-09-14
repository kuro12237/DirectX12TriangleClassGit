#pragma once
#include"Pch.h"
#include"WinApp.h"
#include"MatrixTransform.h"

struct ViewProjection {
	
	Vector3 rotation_ = { 0.0f,0.0f,0.0f };
	Vector3 translation_ = { 0.0f,0.0f,-50.0f };
	Matrix4x4 matView_{};
	Matrix4x4 matProjection_{};

	float fov_ = 0.45f;
	float aspectRatio_ = float(WinApp::GetkCilientHeight()) / float(WinApp::WinApp::GetkCilientWidth());
	
	float nearClip_ = 0.1f;
	float farClip_ = 1000.0f;

	void UpdateMatrix();
};