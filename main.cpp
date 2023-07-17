#include"Game/Game.h"

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{

	Game::Initialize();

	Game::Run();

	Game::Finalize();

	return 0;
}
