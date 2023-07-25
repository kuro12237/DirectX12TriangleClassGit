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
	
	DirectXSetup::CreateDxgiFactory();

	///�f�o�C�X�쐬
	DirectXSetup::CreateDevice();

#ifdef _DEBUG

	DirectXSetup::debugErrorInfoQueue();

#endif // _DEBUG

	///�R�}���h���X�g�̐���
	DirectXSetup::CreateCommands();

	//swapChain
	DirectXSetup::CreateSwapChain(kClientWidth, kClientHeight, WinApp::GetInstance()->GetHwnd());
	
	//rtvDescritor
	DirectXSetup::CreatertvDescritorHeap();
	
	//swapChain����������
	DirectXSetup::CreateSwapChainResorce();
	
	//RTV�̐ݒ�ƍ쐬
	DirectXSetup::SettingandCreateRTV();

	//�t�F���X�̐���
	DirectXSetup::CreateFence();


	ImGuiManager::Initialize(WinApp::GetInstance(), DirectXSetup::GetInstance());

	///���f���̏�����

	//DX�ł��������̂�]��
	
	Model::SetDevice(DirectXSetup::GetInstance()->GetDevice());
	Model::SetCommands(DirectXSetup::GetInstance()->GetCommands());

	//DXC�̏�����
	Model::dxcInitialize();
	//Compile���邽�߂̑Ή�����

	Model::InitializeDfIncludeHandler();

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
	ImGuiManager::BeginFlame(DirectXSetup::GetInstance());

	DirectXSetup::BeginFlame();
	DirectXSetup::ScissorViewCommand(kClientWidth, kClientHeight);
}


void Cleyera::EndFlame()
{
	ImGuiManager::EndFlame(DirectXSetup::GetInstance());
	DirectXSetup::EndFlame();
}


void Cleyera::Finalize()
{
	Camera::Finalize();

	ImGuiManager::Finalize();
	
	Model::Finalize();

	DirectXSetup::Finalize();
	
	WinApp::Finalize();

	DirectXSetup::ReleaseChack();

}


ResourcePeroperty  Cleyera::CreateResource()
{
	ResourcePeroperty resultResource;
	resultResource=Model::GetInstance()->CreateResource();
	return resultResource;
}

void Cleyera::ResourceRelease(ResourcePeroperty Resource)
{
	Model::ResourceRelease(Resource);
}

void Cleyera::TriangleDraw(Vector3 position, int size, unsigned int ColorCode, WorldTransform worldTransform, ResourcePeroperty Resource)
{
	//�J����
	//worldTransform.matWorld_ = Camera::worldViewProjectionMatrixFanc(worldTransform.matWorld_);

	Model::Draw(position,size,ColorCode,worldTransform, Resource);
}
