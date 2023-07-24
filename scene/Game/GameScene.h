#pragma once
#include"../EngineDebug/EngineDebug.h"
#include"../GameBase/GameBase.h"

#include"../EngineDebug/EngineDebug.h"
#include"../../CLEYERA/Cleyera.h"

class GameBase;

class GameScene
{
public:
	GameScene();
	~GameScene();

	static GameScene* GetInstance();

	static void Initialize();

	static void Run();

	static void Finalize();

	static void ChangeState(GameBase* newState);

private:

	GameBase* state_ = nullptr;

	//ウインドウサイズの設定
	const int32_t kClientWidth = 1280;
	const int32_t kClientHeight = 720;

};

