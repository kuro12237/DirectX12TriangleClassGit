#pragma once
#include"../EngineDebug/EngineDebug.h"
#include"../GameBase/GameBase.h"

#include"../EngineDebug/EngineDebug.h"
#include"../../CLEYERA/Cleyera.h"

class GameBase;

class GameSceneManager
{
public:
	GameSceneManager();
	~GameSceneManager();

	static GameSceneManager* GetInstance();

	static void Initialize();

	static void Run();

	static void Finalize();

	static void ChangeState(GameBase* newState);

private:

	GameBase* Stage_ = nullptr;

	//ウインドウサイズの設定
	const int32_t kClientWidth = 1280;
	const int32_t kClientHeight = 720;

};

