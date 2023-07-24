#include"GameSceneManager.h"

GameSceneManager::GameSceneManager(){}

GameSceneManager::~GameSceneManager(){}

GameSceneManager* GameSceneManager::GetInstance()
{
	static GameSceneManager instans;

	return &instans;
}

void GameSceneManager::Initialize()
{
	//Engine初期化
	Cleyera::Initialize(
		GameSceneManager::GetInstance()->kClientWidth,
		GameSceneManager::GetInstance()->kClientHeight
	);

	GameSceneManager::GetInstance()->Stage_ = new EngineDebug();

}

void GameSceneManager::Run()
{

	MSG msg{};

	BYTE key[256] = { 0 };

	///ゲームシーンの初期化

	GameSceneManager::GetInstance()->Stage_->Initialize();

	//ゲーム更新処理
	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			Cleyera::WinMSG(msg);
		}

		Cleyera::BeginFlame(
			GameSceneManager::GetInstance()->kClientWidth,
			GameSceneManager::GetInstance()->kClientHeight);
		
		Input::BeginFlame(key);

		///ゲームシーン
		//更新

		GameSceneManager::GetInstance()->Stage_->Update();

		//更新終了

		//描画

		GameSceneManager::GetInstance()->Stage_->Draw();

		//描画終了

		Cleyera::EndFlame();
	}


}

void GameSceneManager::Finalize()
{
	//頂点などの解放処理
	GameSceneManager::GetInstance()->Stage_->Release();;

	//Engine解放
	Cleyera::Finalize();
}

void GameSceneManager::ChangeState(GameBase* newState)
{
	GameSceneManager::GetInstance()->Stage_ = newState;
}


