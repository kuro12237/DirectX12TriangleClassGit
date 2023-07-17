#include"Camera.h"

Camera::Camera()
{
	Camera_ = {};
	aspectRatio = {};
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
	Camera::GetInstance()->matrixTransform = new MatrixTransform();
	Camera::GetInstance()->vectorTransform = new VectorTransform();

	Camera::GetInstance()->Camera_.Transform = {
		{ 1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{0.0f,0.0f,-5.0f}
	};

	//“ŠŽ‘•ÏŠ·s—ñ
	Camera::GetInstance()->aspectRatio = float(kClientWidth) / float(kClientHeight);
	Camera::GetInstance()->Camera_.ProjectionMatrix =
	Camera::GetInstance()->matrixTransform->
		MakePerspectiveFovMatrix(0.45f, Camera::GetInstance()->aspectRatio, 0.1f, 100.0f);


}

void Camera::Finalize()
{
	delete Camera::GetInstance()->vectorTransform;
	delete Camera::GetInstance()->matrixTransform;
}


void Camera::SetPosition(Transform Transform)
{
	Camera::GetInstance()->Camera_.Transform = Transform;

}

void Camera::Update(Transform transform)
{
	Transform Transform = Camera::GetInstance()->Camera_.Transform;


	Transform.rotate = Camera::GetInstance()->vectorTransform->Add(Transform.rotate, transform.rotate);
	Transform.Scale = Camera::GetInstance()->vectorTransform->Add(Transform.Scale, transform.Scale);
	Transform.translate = Camera::GetInstance()->vectorTransform->Add(Transform.translate, transform.translate);
	
	Camera::GetInstance()->Camera_.Transform = Transform;
	
	
}

Matrix4x4 Camera::worldViewProjectionMatrixFanc(Matrix4x4 matrix)
{
	SCameraPeloperty CameraPloperty = Camera::GetInstance()->Camera_;

	Matrix4x4 Matrix = Camera::GetInstance()->matrixTransform->MakeAffineMatrix(
		CameraPloperty.Transform.Scale,
		CameraPloperty.Transform.rotate,
		CameraPloperty.Transform.translate);

	Matrix4x4 viewMatrix = Camera::GetInstance()->matrixTransform->Inverse(Matrix);

	Matrix4x4 worldViewProjectionMatrix =
		Camera::GetInstance()->matrixTransform->Multiply(matrix, Camera::GetInstance()->
			matrixTransform->Multiply(viewMatrix, CameraPloperty.ProjectionMatrix));

	return worldViewProjectionMatrix;


}
