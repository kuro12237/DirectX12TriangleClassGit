#include"Cleyera.h"

static D3DResourceLeakChecker leakCheck;


int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	Cleyera::Initialize();
    unique_ptr<Model> model = make_unique< Model>();

	model->Initialize(new ModelPlaneState);
	
	WorldTransform worldTransform = {};
	worldTransform.Initialize();


	while (WinApp::WinMsg())
	{
		Cleyera::BeginFlame();
	
		model->Draw(worldTransform);

		Cleyera::EndFlame();

	}

	Cleyera::Finalize();

	return 0;
}