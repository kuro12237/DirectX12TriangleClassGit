#include "DebugCamera.h"

DebugCamera* DebugCamera::GetInstance()
{
	static DebugCamera instance;

	return &instance;
}

void DebugCamera::Initialize()
{
	DebugCamera::GetInstance()->DebugViewProjection_.Initialize();

}

ViewProjection DebugCamera::DebugCameraViewProjection()
{
	ViewProjection result{};
	
	result = DebugCamera::GetInstance()->CameraObjeViewProjection_;

#ifdef _DEBUG
	if (DebugCamera::GetInstance()->isCamera)
	{
		result = DebugCamera::GetInstance()->DebugViewProjection_;
	}
#endif // _DEBUG

	return result;
}

void DebugCamera::execute(ViewProjection viewProjection)
{
	DebugCamera::GetInstance()->CameraObjeViewProjection_ = viewProjection;

	ImGui::Begin("DebugCameraTest");
	ImGui::Checkbox("isDebugCamera", &DebugCamera::GetInstance()->isCamera);
	
	ImGui::End();
    


	DebugCamera::GetInstance()->DebugViewProjection_.UpdateMatrix();

}
