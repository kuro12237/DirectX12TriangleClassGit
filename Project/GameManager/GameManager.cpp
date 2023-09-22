#include "GameManager.h"

GameManager::GameManager()
{
	Cleyera::Initialize();
	Scene_ = new DebugScene();
	Scene_->Initialize(this);

	Grid* grid = new Grid();
	grid->Initialize();
	//GridCommand‚ðƒZƒbƒg
	DebugTools::addCommand(grid,"Grid");

	DebugCamera* debugcamera = new DebugCamera();
	debugcamera->Initialize();
	DebugTools::addCommand(debugcamera,"DebugCamera");
}


GameManager::~GameManager()
{
	DebugTools::ClearCommand();
	delete Scene_;
	Cleyera::Finalize();
}


void GameManager::Run()
{
	while (WinApp::WinMsg())
	{
		Cleyera::BeginFlame();
	
		Scene_->Update(this);
		DebugTools::UpdateExecute(0);
		DebugTools::UpdateExecute(1);

		Scene_->Draw(this);
		DebugTools::DrawExecute(0);
		DebugTools::DrawExecute(1);

		Cleyera::EndFlame();
	}
}

void GameManager::ChangeState(IScene *newScene)
{
	delete Scene_;
	Scene_ = newScene;
	Scene_->Initialize(this);

}
