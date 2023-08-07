#pragma once
#include"WinApp.h"


#include"ViewProjection.h"
#include"WorldTransform.h"
#include"VectorTransform.h"

enum CameraMode
{
	Orthographic,
	PerspectiveFov
};

class Camera
{
public:
	Camera();
	~Camera();

	static Camera* GetInstance();

	static void Initialize();

	static void Finalize();

	static void SetPosition(Transform Transform);

	//static void Update(Transform Transform);

	static Matrix4x4 worldViewProjectionMatrixFanc(Matrix4x4 matrix);

	static Matrix4x4 worldOthographicMatrix(Matrix4x4 matrix);



private:

	ViewProjection viewProjection;
	Transform transform;
};
