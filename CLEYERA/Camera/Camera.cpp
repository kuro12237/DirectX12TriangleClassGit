#include"Camera.h"

Camera::Camera()
{

}

Camera::~Camera()
{

}

Camera* Camera::GetInstance()
{
	static Camera instance;
	return &instance;
}

void Camera::Initialize(const int32_t  kClientWidth, const int32_t  kClientHeight)
{

	Camera::GetInstance()->transform = {
		{ 1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{0.0f,0.0f,-5.0f}
	};

	//“ŠŽ‘•ÏŠ·s—ñ
	Camera::GetInstance()->aspectRatio = float(kClientWidth) / float(kClientHeight);
	Camera::GetInstance()->viewProjection.matProjection =
		MatrixTransform::MakePerspectiveFovMatrix(0.45f, Camera::GetInstance()->aspectRatio, 0.1f, 100.0f);


}


void Camera::SetPosition(Transform Transform)
{
	Camera::GetInstance()->transform = Transform;

}



Matrix4x4 Camera::worldViewProjectionMatrixFanc(Matrix4x4 matrix)
{
	
	Matrix4x4 Matrix = MatrixTransform::MakeAffineMatrix(
		Camera::GetInstance()->transform.Scale,
		Camera::GetInstance()->transform.rotate,
		Camera::GetInstance()->transform.translate);

	Matrix4x4 viewMatrix = MatrixTransform::Inverse(Matrix);

	Matrix4x4 worldViewProjectionMatrix =
		MatrixTransform::Multiply(matrix, 
			MatrixTransform::Multiply(viewMatrix,
				Camera::GetInstance()->viewProjection.matProjection));

	return worldViewProjectionMatrix;


}

