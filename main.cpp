#include"Cleyera.h"



static D3DResourceLeakChecker leakCheck;

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	Cleyera::Initialize();
    unique_ptr<Model> model = make_unique< Model>();

	uint32_t texHandle = TextureManager::LoadTexture("Resources/uvChecker.png");
	model->Initialize(new ModelPlaneState);
	model->SetTexHandle(texHandle);
	
	WorldTransform worldTransform = {};
	worldTransform.Initialize();

	ViewProjection viewProjection;
	viewProjection.Initialize({ 0.235f,-0.60f,0.0f }, { 11.0f,5.1f,-15 });
	//viewProjection.Initialize();
	texHandle;
	Vector3 uvScale = { 1,1,1, };
	Vector3 uvrotate = {};
	Vector3 uvtranslate = {};

	while (WinApp::WinMsg())
	{
		Cleyera::BeginFlame();
	
		ImGui::Begin("Debugcamera");
		
	
		ImGui::SliderFloat3("trans", &viewProjection.translation_.x, -15, 15);
		ImGui::SliderFloat3("rotate", &viewProjection.rotation_.x, -1, 1);
		ImGui::End();

		
		
		
		ImGui::Begin("Plane");
		ImGui::DragFloat3("scale", &worldTransform.scale.x, 0.1f);
		ImGui::DragFloat3("rotate", &worldTransform.rotation.x, 0.1f);
		ImGui::DragFloat3("trans", &worldTransform.translate.x, 0.1f);
		ImGui::DragFloat3("uvScale", &worldTransform.scale.x, 0.1f);
		ImGui::Text("uv");
		ImGui::DragFloat3("uvRotate", &worldTransform.rotation.x, 0.1f);
		ImGui::DragFloat3("uvscale", &uvScale.x, 0.1f);
		ImGui::DragFloat3("uvrotate", &uvrotate.x, 0.1f);
		ImGui::DragFloat3("uvTrans", &uvtranslate.x, 0.1f);
		ImGui::End();

		model->SetUvRotate(uvrotate);
		model->SetUvScale(uvScale);
		model->SetUvTranslate(uvtranslate);

		worldTransform.translate.z += 0.01f;
		worldTransform.UpdateMatrix();
		


		viewProjection.UpdateMatrix();

		DebugTools::SetViewProjection(viewProjection);
		DebugTools::Execute(0);
	


		model->Draw(worldTransform,viewProjection);

		Cleyera::EndFlame();

	}

	Cleyera::Finalize();

	return 0;
}