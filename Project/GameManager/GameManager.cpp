#include "GameManager.h"

GameManager::GameManager()
{
	Cleyera::Initialize();
	Scene_ = new DebugScene();
	Scene_->Initialize(this);

	Grid* grid = new Grid();
	Grid::Initialize();
	//GridCommand‚ðƒZƒbƒg
	DebugTools::addCommand(grid);
	DebugCamera* debugcamera = new DebugCamera();
	DebugCamera::Initialize();
	DebugTools::addCommand(debugcamera);
}


GameManager::~GameManager()
{
	delete Scene_;
	Cleyera::Finalize();
}

void GameManager::Run()
{
	while (WinApp::WinMsg())
	{
		Cleyera::BeginFlame();
		
		Scene_->Update(this);
		
		DebugTools::Execute(0);
		DebugTools::Execute(1);
		Scene_->Draw(this);

		Cleyera::EndFlame();
	}
}

void GameManager::ChangeState(IScene *newScene)
{
	delete Scene_;
	Scene_ = newScene;
	Scene_->Initialize(this);

}
