#include"Cleyera.h"


int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{

	///������
	//�E�C���h�E�T�C�Y�̐ݒ�
	const int32_t kClientWidth = 1280;
	const int32_t kClientHeight = 720;

	Cleyera::Initialize(kClientWidth, kClientHeight);
	
	MatrixTransform *matrixTransform_ = new MatrixTransform();


	Vector3 pos_ = { 0,0,0 };
	ResourcePeroperty resource_=Cleyera::CreateResource();
	unsigned int color_=BLACK;
	WorldTransform worldTransform_;
	
	worldTransform_.matWorld_=matrixTransform_->Identity();
	float size = 1;

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
		
	
		
		worldTransform_.UpdateMatrix();
		//�X�V�I��

		//�`��
		Cleyera::TriangleDraw(
			pos_,
			size,
			color_,
			worldTransform_, 
			resource_);
		
		//�`��I��

		Cleyera::EndFlame();
	}

	//���_�Ȃǂ̃Q�[���V�[���̉������
	Cleyera::ResourceRelease(resource_);


	Cleyera::Finalize();
	return 0;
}
