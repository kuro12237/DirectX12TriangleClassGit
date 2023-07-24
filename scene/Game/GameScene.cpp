#include"GameScene.h"

GameScene::GameScene(){}

GameScene::~GameScene(){}

GameScene* GameScene::GetInstance()
{
	static GameScene instans;

	return &instans;
}

void GameScene::Initialize()
{
	//Engine������
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

	///�Q�[���V�[���̏�����

	GameScene::GetInstance()->Stage_->Initialize();

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

		GameScene::GetInstance()->Stage_->Update();

		//�X�V�I��

		//�`��

		GameScene::GetInstance()->Stage_->Draw();

		//�`��I��

		Cleyera::EndFlame();
	}


}

void GameScene::Finalize()
{
	//���_�Ȃǂ̉������
	GameScene::GetInstance()->Stage_->Release();;

	//Engine���
	Cleyera::Finalize();
}

void GameScene::ChangeState(GameBase* newState)
{
	GameScene::GetInstance()->Stage_ = newState;
}


