#include "DebugCamera.h"

DebugCamera* DebugCamera::GetInstance()
{
	static DebugCamera instance;

	return &instance;
}

void DebugCamera::Initialize()
{
	DebugCamera::GetInstance()->DebugViewProjection_.Initialize({0.0f,0.0f,0.0f},{0.0f,3.0f,-15.0f});
	DebugCamera::GetInstance()->matRotate_ = MatrixTransform::Identity();
	DebugCamera::GetInstance()->worldTransform_.Initialize();
	DebugCamera::GetInstance()->DebugViewProjection_.UpdateMatrix();
}

void DebugCamera::IsUseDebugCameraViewProjection()
{

#ifdef _DEBUG
	DebugTools::SetIsviewProjectionReWrite(false);
	if (DebugCamera::GetInstance()->isCamera)
	{

		DebugTools::SetViewProjection(DebugCamera::GetInstance()->DebugViewProjection_);
		DebugTools::SetIsviewProjectionReWrite(true);
	}
#endif // _DEBUG

	
}

void DebugCamera::TranslateMove()
{
	Vector3 speed = DebugCamera::GetInstance()->CemeraMoveSpeed_;

	//←
	if (Input::GetInstance()->PushKey(DIK_A))
	{
		DebugCamera::GetInstance()->offset_.x -= speed.x; 
	}//→
	else if (Input::GetInstance()->PushKey(DIK_D))
	{
		DebugCamera::GetInstance()->offset_.x += speed.x;
	}

	//↑
	if (Input::GetInstance()->PushKey(DIK_W))
	{
		DebugCamera::GetInstance()->offset_.y += speed.y;
	}//↓
	else if (Input::GetInstance()->PushKey(DIK_S))
	{
		DebugCamera::GetInstance()->offset_.y -= speed.y;
	}
}

void DebugCamera::RotateMove()
{
	Matrix4x4 matRotate = MatrixTransform::Identity();
	Vector3 speed = DebugCamera::GetInstance()->CemaraRotateSpeed_;
    
	//←の回転
	if (Input::GetInstance()->PushKey(DIK_LEFT))
	{
		matRotate = MatrixTransform::Multiply(matRotate, MatrixTransform::RotateYMatrix(speed.y));
	}//→の回転
	else if(Input::GetInstance()->PushKey(DIK_RIGHT))
	{
		matRotate = MatrixTransform::Multiply(matRotate, MatrixTransform::RotateYMatrix(-speed.y));
	}

	//↑の回転
	if (Input::GetInstance()->PushKey(DIK_UP))
	{
		matRotate = MatrixTransform::Multiply(matRotate, MatrixTransform::RotateXMatrix(speed.x));
	}//↓の回転
	else if (Input::GetInstance()->PushKey(DIK_DOWN))
	{
		matRotate = MatrixTransform::Multiply(matRotate, MatrixTransform::RotateXMatrix(-speed.x));
	}

	DebugCamera::GetInstance()->matRotate_ = MatrixTransform::Multiply(matRotate, DebugCamera::GetInstance()->matRotate_);
}

void DebugCamera::UpdateExecute()
{
	DebugCamera::GetInstance()->CameraObjeViewProjection_ = DebugTools::GetInstance()->GetViewProjection();

	ImGui::Begin("DebugCameraTest");
	ImGui::Checkbox("isDebugCamera", &DebugCamera::GetInstance()->isCamera);
	ImGui::SliderFloat3("CameraRotate",&DebugCamera::GetInstance()->CemaraRotateSpeed_.x,-0.1f,0.1f);
	ImGui::Text("WASD:Move");
	ImGui::Text("TOP,DOWN,LEFT,RIGHT : CameraRotate");
	ImGui::End();

	IsUseDebugCameraViewProjection();

	//中心地点からずらす
	Vector3 offset = DebugCamera::GetInstance()->offset_;
	offset = VectorTransform::TransformNormal(offset, DebugCamera::GetInstance()->matRotate_);
	DebugCamera::GetInstance()->worldTransform_.translate = offset;


#pragma region 更新処理
	//Translateの更新
	Matrix4x4 translateMatrix =
		MatrixTransform::TranslateMatrix(DebugCamera::GetInstance()->worldTransform_.translate);
	//worldTransformの更新
	DebugCamera::GetInstance()->worldTransform_.matWorld =
		MatrixTransform::Multiply(DebugCamera::GetInstance()->matRotate_, translateMatrix);
	//view行列の更新
	DebugCamera::GetInstance()->DebugViewProjection_.matView_ =
		MatrixTransform::Multiply(MatrixTransform::Inverse(translateMatrix),
			MatrixTransform::Inverse(DebugCamera::GetInstance()->matRotate_));
#pragma endregion 

	TranslateMove();
	RotateMove();

}

void DebugCamera::DrawExecute(ViewProjection viewProjection)
{
	viewProjection;
}
