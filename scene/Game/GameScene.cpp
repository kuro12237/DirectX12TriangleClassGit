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
	//Engin������
	Cleyera::Initialize(
		GameScene::GetInstance()->kClientWidth,
		GameScene::GetInstance()->kClientHeight
	);


	GameScene::GetInstance()->state_ = new EngineDebug();

}

void GameScene::Run()
{

	MSG msg{};

	BYTE key[256] = { 0 };

	///�Q�[���V�[���̏�����

	GameScene::GetInstance()->state_->Initialize(GameScene::GetInstance());

	//�Q�[���X�V����
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



		///�Q�[���V�[��
		//�X�V

		GameScene::GetInstance()->state_->Update(GameScene::GetInstance());

		//�X�V�I��

		
		//�`��

		GameScene::GetInstance()->state_->Draw(GameScene::GetInstance());

		//�`��I��


		Cleyera::EndFlame();
	}


}

void GameScene::Finalize()
{
	//���_�Ȃǂ̉������
	GameScene::GetInstance()->state_->Release(GameScene::GetInstance());;

	//Engine���
	Cleyera::Finalize();
}

void GameScene::ChangeState(GameBase* newState)
{
	GameScene::GetInstance()->state_ = newState;
}


