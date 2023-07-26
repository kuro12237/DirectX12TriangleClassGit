#include"CLEYERA.h"

Cleyera::Cleyera(){}

Cleyera::~Cleyera(){}

void Cleyera::Initialize(const int32_t  kClientWidth, const int32_t  kClientHeight)
{
	//WinApp�̏�����
	
	WinApp::Initialize(kClientWidth, kClientHeight);

	//
	////DirectX�̏�����
	//
	
	///�t�@�N�g���[�쐬
	
	DirectXCommon::CreateDxgiFactory();

	///�f�o�C�X�쐬
	DirectXCommon::CreateDevice();

#ifdef _DEBUG

	DirectXCommon::debugErrorInfoQueue();

#endif // _DEBUG

	///�R�}���h���X�g�̐���
	DirectXCommon::CreateCommands();

	//swapChain
	DirectXCommon::CreateSwapChain(kClientWidth, kClientHeight, WinApp::GetInstance()->GetHwnd());
	
	//rtvDescritor
	DirectXCommon::CreatertvDescritorHeap();
	
	//swapChain����������
	DirectXCommon::CreateSwapChainResorce();
	
	//RTV�̐ݒ�ƍ쐬
	DirectXCommon::SettingandCreateRTV();

	//�t�F���X�̐���
	DirectXCommon::CreateFence();


	ImGuiManager::Initialize(WinApp::GetInstance(), DirectXCommon::GetInstance());

	///���f���̏�����

	//DX�ł��������̂�]��
	
	Model::SetDevice(DirectXCommon::GetInstance()->GetDevice());
	Model::SetCommands(DirectXCommon::GetInstance()->GetCommands());

	//DXC�̏�����
	Model::dxcInitialize();
	//Compile���邽�߂̑Ή�����

	Model::DefaultIncludeHandlerInitialize();

	///�V�F�[�_�[�R���p�C������
	Model::CompileShaders();
	
	//�}�`�`��̃p�C�v���C��
	Model::ShapeCreatePSO();
	
	
	//�R���p�C�������V�F�[�_�[�̉��
	Model::ShaderRelease();
	
	//�J�����̏�����
	Camera::Initialize(kClientWidth,kClientHeight);
}


void Cleyera::WinMSG(MSG msg)
{
	WinApp::Msg(msg);
}


void Cleyera::BeginFlame(const int32_t kClientWidth, const int32_t kClientHeight)
{
	ImGuiManager::BeginFlame(DirectXCommon::GetInstance());

	DirectXCommon::BeginFlame();
	DirectXCommon::ScissorViewCommand(kClientWidth, kClientHeight);
}


void Cleyera::EndFlame()
{
	ImGuiManager::EndFlame(DirectXCommon::GetInstance());
	DirectXCommon::EndFlame();
}


void Cleyera::Finalize()
{
	Camera::Finalize();

	ImGuiManager::Finalize();
	
	Model::Finalize();

	DirectXCommon::Finalize();
	
	WinApp::Finalize();

	DirectXCommon::ReleaseChack();

}

//ResourcePeroperty  Cleyera::CreateResource()
//{
//	//ResourcePeroperty resultResource;
//	//resultResource=Model::GetInstance()->CreateResource();
//	//return resultResource;
//}

void Cleyera::ResourceRelease(ResourcePeroperty Resource)
{
	Resource;
	//Model::ResourceRelease(Resource);
}

void Cleyera::TriangleDraw(Vector3 position, float size, unsigned int ColorCode, WorldTransform worldTransform, ResourcePeroperty Resource)
{
	//�J����
	worldTransform.matWorld_ = Camera::worldViewProjectionMatrixFanc(worldTransform.matWorld_);
	position, size, ColorCode, Resource;
	//Model::Draw(position,size,ColorCode,worldTransform, Resource);
}
