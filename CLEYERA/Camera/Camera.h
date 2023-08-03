#pragma once
#include"WinApp.h"


#include"ViewProjection.h"
#include"WorldTransform.h"
#include"VectorTransform.h"


class Camera
{
public:
	Camera();
	~Camera();

	static Camera* GetInstance();

	static void Initialize(const int32_t kClientWidth, const int32_t kClientHeight);

	static void Update(WorldTransform worldTransform);

	static Matrix4x4 worldViewProjectionmatrixFanc(Matrix4x4 m);

private:

	ViewProjection viewProjection_;

	WorldTransform worldTransform_;
};

