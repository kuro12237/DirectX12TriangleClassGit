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
	//Engin������
	Cleyera::Initialize(
		Game::GetInstance()->kClientWidth,
		Game::GetInstance()->kClientHeight
	);

	///�Q�[���V�[���̏�����

	Game::GetInstance()->state_ = new EngineDebug();

	Game::GetInstance()->state_->Initialize(Game::GetInstance());
}

void Game::Run()
{
	MSG msg{};

	BYTE key[256] = { 0 };

	//�Q�[���X�[�v
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



		///�Q�[���V�[��
		//�X�V

		Game::GetInstance()->state_->Update(Game::GetInstance());

		//�X�V�I��

		
		//�`��

		Game::GetInstance()->state_->Draw(Game::GetInstance());

		//�`��I��


		Cleyera::EndFlame();
	}


}

void Game::Finalize()
{
	//���_�Ȃǂ̉������
	Game::GetInstance()->state_->Release(Game::GetInstance());;

	//Engine���
	Cleyera::Finalize();
}

void Game::ChangeState(GameScene* newState)
{
	Game::GetInstance()->state_ = newState;
}


