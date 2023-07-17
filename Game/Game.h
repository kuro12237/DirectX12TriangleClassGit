#pragma once
#include"../CLEYERA/CLEYERA.h"
#include"../scene/GameScene/GameScene.h"

#include"../scene/EngineDebug/EngineDebug.h"

class GameScene;

class Game
{
public:
	Game();
	~Game();

	static Game* GetInstance();

	static void Initialize();

	static void Run();

	static void Finalize();

	static void ChangeState(GameScene* newState);

private:

	GameScene* state_ = nullptr;

	//ウインドウサイズの設定
	const int32_t kClientWidth = 1280;
	const int32_t kClientHeight = 720;

};

