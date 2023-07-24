#include"scene/Game/GameScene.h"


int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{

	GameScene::Initialize();

	GameScene::Run();

	GameScene::Finalize();

	return 0;
}
