#include"Game.h"
#include"../CLEYERA/CLEYERA.h"

Game::Game()
{
}

Game::~Game()
{
}

Game* Game::GetInstance()
{
	static Game instans;

	return &instans;
}

void Game::Initialize()
{
	//Engin初期化
	Cleyera::Initialize(
		Game::GetInstance()->kClientWidth,
		Game::GetInstance()->kClientHeight
	);

	///ゲームシーンの初期化

	Game::GetInstance()->state_ = new EngineDebug();

	Game::GetInstance()->state_->Initialize(Game::GetInstance());
}

void Game::Run()
{
	MSG msg{};

	BYTE key[256] = { 0 };

	//ゲームスープ
	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			Cleyera::WinMSG(msg);
		}

		Cleyera::BeginFlame(
			Game::GetInstance()->kClientWidth,
			Game::GetInstance()->kClientHeight);
		Input::BeginFlame(key);



		///ゲームシーン
		//更新

		Game::GetInstance()->state_->Update(Game::GetInstance());

		//更新終了

		
		//描画

		Game::GetInstance()->state_->Draw(Game::GetInstance());

		//描画終了


		Cleyera::EndFlame();
	}


}

void Game::Finalize()
{
	//頂点などの解放処理
	Game::GetInstance()->state_->Release(Game::GetInstance());;

	//Engine解放
	Cleyera::Finalize();
}

void Game::ChangeState(GameScene* newState)
{
	Game::GetInstance()->state_ = newState;
}


