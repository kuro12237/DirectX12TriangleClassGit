#include"scene/Manager/GameSceneManager.h"


int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{

	GameSceneManager::Initialize();

	GameSceneManager::Run();

	GameSceneManager::Finalize();

	return 0;
}
