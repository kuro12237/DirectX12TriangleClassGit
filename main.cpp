#include"Cleyera.h"

static D3DResourceLeakChecker leakCheck;


int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	Cleyera::Initialize();
    unique_ptr<Model> model = make_unique< Model>();

	model->Initialize(new ModelLineState,{-1,0,0,1},{1,0,0,1});
	
	WorldTransform worldTransform = {};
	worldTransform.Initialize();

	ViewProjection *viewProjection = new ViewProjection;
	viewProjection->Initialize({ 0,0,0 }, { 0,0,-5 });

	while (WinApp::WinMsg())
	{
		Cleyera::BeginFlame();
	
		viewProjection->UpdateMatrix();
		
		worldTransform.translate.z += 0.01f;
		worldTransform.UpdateMatrix();
		model->Draw(worldTransform,viewProjection);

		Cleyera::EndFlame();

	}

	Cleyera::Finalize();

	return 0;
}