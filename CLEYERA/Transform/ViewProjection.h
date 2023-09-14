#pragma once
#include"Pch.h"
#include"WinApp.h"
#include"MatrixTransform.h"

class ViewProjection {
	
public:
	void Initialize(Vector3 r={0.0f,0.0f,0.0f},Vector3 t={0.0f,0.0f,-5.0f});

	void UpdateMatrix();

	Matrix4x4  GetMatProjection() { return matProjection_; }

	Matrix4x4 GetViewMatrix() { return matView_; }

private:
	Vector3 rotation_ = { 0.0f,0.0f,0.0f };
	Vector3 translation_ = { 0.0f,0.0f,-15.0f };
	Matrix4x4 matView_{};
	Matrix4x4 matProjection_{};

	float fov_ = 0.45f;
	float aspectRatio_ = float(WinApp::GetkCilientWidth()) / float(WinApp::WinApp::GetkCilientHeight());
	
	float nearClip_ = 0.1f;
	float farClip_ = 100.0f;

	
};