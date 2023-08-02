#include"CLEYERA.h"


Cleyera::Cleyera()
{
}


Cleyera::~Cleyera()
{
}


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

	Input::DirectInputObjCreate();
	Input::KeyDeviceCreate();


	ImGuiManager::Initialize(WinApp::GetInstance(), DirectXSetup::GetInstance());

	TexManager::Initialize();

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
	
	//Tex�`��̃p�C�v���C��
	Model::SpriteCreatePSO();

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
	
	TexManager::Finalize();

	Model::Finalize();
	DirectXSetup::Finalize();
	
	WinApp::Finalize();

	DirectXSetup::ReleaseChack();

}


texResourceProperty Cleyera::LoadTex(const std::string& filePath)
{
	texResourceProperty tex;

	tex = TexManager::LoadTexture(filePath, DirectXSetup::GetInstance());

	return tex;
}


ResourcePeroperty Cleyera::CreateSpriteResource()
{
	ResourcePeroperty ResultResource;
	
	ResultResource = Model::CreateTriangleSpriteResource();
	return ResultResource;
}


void Cleyera::SpriteTriangleResourceRelease(ResourcePeroperty &Resource, texResourceProperty &tex)
{
	Model::TriangleSpriteResourceRelease(Resource, tex);
}



ResourcePeroperty  Cleyera::CreateShapeResource()
{
	ResourcePeroperty resultResource;
	resultResource=Model::GetInstance()->CreateShapeResource();
	return resultResource;
}


void Cleyera::TriangleResourceRelease(ResourcePeroperty Resource)
{
	Model::ShapeResourceRelease(Resource);
}


void Cleyera::TriangleDraw(Position position, Vector4 color, Matrix4x4 worldTransform, ResourcePeroperty Resource)
{
	Matrix4x4 m = Camera::worldViewProjectionMatrixFanc(worldTransform);
	Model::ShapeDraw(position, color,m, Resource);
}


void Cleyera::SpriteTriangleDraw(Position position, unsigned int color, Matrix4x4 worldTransform, ResourcePeroperty Resource, texResourceProperty tex)
{
	Matrix4x4 m = Camera::worldViewProjectionMatrixFanc(worldTransform);
	Model::TriangleSpriteDraw(position, color, m, Resource, tex);
}