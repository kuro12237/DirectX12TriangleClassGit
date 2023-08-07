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

void Camera::Initialize()
{

	Camera::GetInstance()->transform = {
		{ 1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{0.0f,0.0f,-5.0f}
	};

	//“ŠŽ‘•ÏŠ·s—ñ
	Camera::GetInstance()->viewProjection.aspectRatio = 
		float(WinApp::GetInstance()->GetkClinentWidth()) 
		/
		float(WinApp::GetInstance()->GetkClinentHeight());

	Camera::GetInstance()->viewProjection.nearClip = 0.1f;
	Camera::GetInstance()->viewProjection.farClip = 100.0f;
	Camera::GetInstance()->viewProjection.fov = 0.45f;

	Camera::GetInstance()->viewProjection.matProjection =
		MatrixTransform::MakePerspectiveFovMatrix(
			Camera::GetInstance()->viewProjection.fov,
			Camera::GetInstance()->viewProjection.aspectRatio,
			Camera::GetInstance()->viewProjection.nearClip,
			Camera::GetInstance()->viewProjection.farClip
		);
}

void Camera::Finalize()
{
	

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

Matrix4x4 Camera::worldOthographicMatrix(Matrix4x4 matrix)
{

	Matrix4x4 viewMatrix = MatrixTransform::Identity();
	
	Matrix4x4 OrthographicMatrix = MatrixTransform::MakeOrthographicMatrix(
		0.0f, 0.0f,
		float(WinApp::GetkClinentWidth()),
		float(WinApp::GetkClinentHeight()),
		0.0f,
		100.0f
	);

	Matrix4x4 worldViewProjectionMatrix =
		MatrixTransform::Multiply(matrix,
			MatrixTransform::Multiply(viewMatrix,
				OrthographicMatrix));

	return worldViewProjectionMatrix;

}

