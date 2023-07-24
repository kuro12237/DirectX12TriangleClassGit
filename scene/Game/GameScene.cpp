#include"GameScene.h"

GameScene::GameScene()
{
}

GameScene::~GameScene()
{
}

GameScene* GameScene::GetInstance()
{
	static GameScene instans;

	return &instans;
}

void GameScene::Initialize()
{
	//Engin初期化
	Cleyera::Initialize(
		GameScene::GetInstance()->kClientWidth,
		GameScene::GetInstance()->kClientHeight
	);


	GameScene::GetInstance()->Stage_ = new EngineDebug();

}

void GameScene::Run()
{

	MSG msg{};

	BYTE key[256] = { 0 };

	///ゲームシーンの初期化

	GameScene::GetInstance()->Stage_->Initialize(GameScene::GetInstance());

	//ゲーム更新処理
	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			Cleyera::WinMSG(msg);
		}

		Cleyera::BeginFlame(
			GameScene::GetInstance()->kClientWidth,
			GameScene::GetInstance()->kClientHeight);
		
		Input::BeginFlame(key);



		///ゲームシーン
		//更新

		GameScene::GetInstance()->Stage_->Update(GameScene::GetInstance());

		//更新終了

		
		//描画

		GameScene::GetInstance()->Stage_->Draw(GameScene::GetInstance());

		//描画終了


		Cleyera::EndFlame();
	}


}

void GameScene::Finalize()
{
	//頂点などの解放処理
	GameScene::GetInstance()->Stage_->Release(GameScene::GetInstance());;

	//Engine解放
	Cleyera::Finalize();
}

void GameScene::ChangeState(GameBase* newState)
{
	GameScene::GetInstance()->Stage_ = newState;
}


