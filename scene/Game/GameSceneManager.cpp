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
	//Engine������
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

	///�Q�[���V�[���̏�����

	GameSceneManager::GetInstance()->Stage_->Initialize();

	//�Q�[���X�V����
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

		///�Q�[���V�[��
		//�X�V

		GameSceneManager::GetInstance()->Stage_->Update();

		//�X�V�I��

		//�`��

		GameSceneManager::GetInstance()->Stage_->Draw();

		//�`��I��

		Cleyera::EndFlame();
	}


}

void GameSceneManager::Finalize()
{
	//���_�Ȃǂ̉������
	GameSceneManager::GetInstance()->Stage_->Release();;

	//Engine���
	Cleyera::Finalize();
}

void GameSceneManager::ChangeState(GameBase* newState)
{
	GameSceneManager::GetInstance()->Stage_ = newState;
}


