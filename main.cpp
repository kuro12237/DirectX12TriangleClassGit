#include"Cleyera.h"

static D3DResourceLeakChecker leakCheck;


int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	Cleyera::Initialize();
    unique_ptr<Model> model = make_unique< Model>();

	model->Initialize(new ModelPlaneState,{-1,0,0,1},{1,0,0,1});
	
	WorldTransform worldTransform = {};
	worldTransform.Initialize();

	ViewProjection viewProjection;
	viewProjection.Initialize({ 0.235f,-0.60f,0.0f }, { 11.0f,5.1f,-15 });
	DebugTools::Initialize();

	while (WinApp::WinMsg())
	{
		Cleyera::BeginFlame();
	
	
		ImGui::Begin("Debugcamera");
		
	
		ImGui::SliderFloat3("trans", &viewProjection.translation_.x, -15, 15);
		ImGui::SliderFloat3("rotate", &viewProjection.rotation_.x, -1, 1);
		
		ImGui::End();

		viewProjection.UpdateMatrix();
		DebugTools::SetViewProjection(viewProjection);
		DebugTools::Execute(0);

		worldTransform.translate.z += 0.01f;
		worldTransform.UpdateMatrix();
		model->Draw(worldTransform,viewProjection);

		Cleyera::EndFlame();

	}

	Cleyera::Finalize();

	return 0;
}