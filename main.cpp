#include"Cleyera.h"
#define TRIANGLE_MAX 2


int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{

	///������
	//�E�C���h�E�T�C�Y�̐ݒ�
	const int32_t kClientWidth = 1280;
	const int32_t kClientHeight = 720;

	Cleyera::Initialize(kClientWidth, kClientHeight);
	
	
	Model* model[TRIANGLE_MAX];

	Vector3 pos_[TRIANGLE_MAX];
	WorldTransform worldTransform_[TRIANGLE_MAX];

	pos_[0] = {-0.5f,0.0f,0};
	pos_[1] = { 0.5f,0.0f,0};


	//model
	for (int i = 0; i < TRIANGLE_MAX; i++)
	{
		model[i] = new Model();

		model[i]->Initialize();
	}



	for (int i = 0; i < TRIANGLE_MAX; i++)
	{
		
		model[i]->SetPos(pos_[i]);
		model[i]->SetSize(0.5f);
	}
	
	MSG msg{};

	//�Q�[���X�V����
	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			Cleyera::WinMSG(msg);
		}

		Cleyera::BeginFlame(kClientWidth,kClientHeight);


		///�Q�[���V�[��
		//�X�V
		
	
		
		//�X�V�I��

		//�`��

		
		for (int i = 0; i < TRIANGLE_MAX; i++)
		{
			model[i]->Draw();
		}
		//�`��I��

		Cleyera::EndFlame();
	}

	//���_�Ȃǂ̂̉������
	for (int i = 0; i < TRIANGLE_MAX; i++)
	{
		model[i]->ResourceRelease();
	}

	Cleyera::Finalize();
	return 0;
}
