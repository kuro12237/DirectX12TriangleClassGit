#include "Camera.h"


Camera::Camera()
{
}

Camera::~Camera()
{
}

Camera *Camera::GetInstance()
{
	static Camera instance;

	return &instance;
}

void Camera::Initialize(const int32_t kClientWidth,const int32_t kClientHeight)
{
	Camera::GetInstance()->worldTransform_.Initialize();
	Camera::GetInstance()->worldTransform_.translation_ = { 0.0f,0.0f,-5.0f };
	Camera::GetInstance()->worldTransform_.matWorld =
		MatrixTransform::Identity();

	Camera::GetInstance()->viewProjection_.aspectRatio =
		float(kClientWidth / kClientHeight);

	Camera::GetInstance()->viewProjection_.fov = 0.45f;
	Camera::GetInstance()->viewProjection_.nearClip = 0.1f;
	Camera::GetInstance()->viewProjection_.farClip = 100.0f;

	Camera::GetInstance()->viewProjection_.matProjection =
		MatrixTransform::MakePerspectiveFovMatrix(
			Camera::GetInstance()->viewProjection_.fov,
			Camera::GetInstance()->viewProjection_.aspectRatio,
			Camera::GetInstance()->viewProjection_.nearClip,
			Camera::GetInstance()->viewProjection_.farClip
		);

	
}

void Camera::Update(WorldTransform worldTransform)
{
	//t
	Camera::GetInstance()->worldTransform_.translation_ =
	  VectorTransform::Add(
		  Camera::GetInstance()->worldTransform_.translation_,
		  worldTransform.translation_
	 );
	//r
	Camera::GetInstance()->worldTransform_.rotate_ =
		VectorTransform::Add(
			Camera::GetInstance()->worldTransform_.rotate_,
			worldTransform.rotate_
		);
	//s
	Camera::GetInstance()->worldTransform_.scale_ =
		VectorTransform::Add(
			Camera::GetInstance()->worldTransform_.scale_,
			worldTransform.scale_
		);

	Camera::GetInstance()->worldTransform_.matWorld =
		MatrixTransform::MakeAffineMatrix(
			Camera::GetInstance()->worldTransform_.scale_,
			Camera::GetInstance()->worldTransform_.rotate_,
			Camera::GetInstance()->worldTransform_.translation_
		);


}

Matrix4x4 Camera::worldViewProjectionmatrixFanc(Matrix4x4 m)
{
	ViewProjection viewProjection = 
		Camera::GetInstance()->viewProjection_;

	WorldTransform worldTransform = 
		Camera::GetInstance()->worldTransform_;

	Matrix4x4 cworldMatrix = MatrixTransform::MakeAffineMatrix(
		worldTransform.scale_,
		worldTransform.rotate_,
		worldTransform.translation_);

	Matrix4x4 viewMatrix =  
		MatrixTransform::Inverse(cworldMatrix);

	Matrix4x4 worldVPMatrix =
		MatrixTransform::Multiply(
			m,
			MatrixTransform::Multiply(viewMatrix,
			Camera::GetInstance()->viewProjection_.matProjection)
		);
	

	return worldVPMatrix;

}
