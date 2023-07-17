#include"CLEYERA.h"

Cleyera::Cleyera()
{
}

Cleyera::~Cleyera()
{
}

void Cleyera::Initialize(const int32_t  kClientWidth, const int32_t  kClientHeight)
{

	WinSetup = new WindowsSetup();
	DXSetup = new DirectXSetup();
	model = new Model();

	//WinSetup�̏�����
	

	WinSetup->Initialize(kClientWidth, kClientHeight);

	//
	////DirectX�̏�����
	//

	///�t�@�N�g���[�쐬
	DXSetup->CreateDXGIFactorye();

	///�f�o�C�X�쐬
	DXSetup->CreateDevice();

#ifdef _DEBUG

	DXSetup->debugErrorInfoQueue();

#endif // _DEBUG

	///�R�}���h���X�g�̐���
	DXSetup->CreateCommands();

	//swapChain
	DXSetup->CreateSwapChain(kClientWidth, kClientHeight, WinSetup->GetHwnd());
	//rtvDescritor
	DXSetup->CreatertvDescritorHeap();
	//swapChain����������
	DXSetup->CreateSwapChainResorce();
	//RTV�̐ݒ�ƍ쐬
	DXSetup->SettingandCreateRTV();
	//�t�F���X�̐���
	DXSetup->CreateFence();

	///���f���̏�����

	//DX�ł��������̂�]��
	model->SetDevice(DXSetup->GetDevice());
	model->SetCommands(DXSetup->GetCommands());

	//DXC�̏�����
	model->dxcInitialize();
	//Compile���邽�߂̑Ή�����
	model->InitializeDfIncludeHandler();
	
	///�V�F�[�_�[�R���p�C������
	model->CompileShaders();

	//�}�`�`��̃p�C�v���C��
	model->ShapeCreatePSO();



	model->ShaderRelease();
}


void Cleyera::WinMSG(MSG msg)
{
	WinSetup->WinMSG(msg);

}

void Cleyera::BeginFlame(const int32_t kClientWidth, const int32_t kClientHeight)
{

	DXSetup->BeginFlame();
	DXSetup->ScissorViewCommand(kClientWidth, kClientHeight);

}

void Cleyera::EndFlame()
{
	DXSetup->EndFlame();
}

ResourcePeroperty  Cleyera::CreateResource()
{
	ResourcePeroperty resultResource;
	resultResource=model->CreateResource();
	return resultResource;
}

void Cleyera::TriangleDraw(Position position, unsigned int ColorCode, Matrix4x4 worldTransform, ResourcePeroperty Resource)
{
	model->ShapeDraw(position, ColorCode,worldTransform, Resource);

}



