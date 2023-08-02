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
	
	DirectXCommon::CreateDxgiFactory();

	///�f�o�C�X�쐬
	DirectXCommon::CreateDevice();

#ifdef _DEBUG

	DirectXSetup::debugErrorInfoQueue();

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

	ImGuiManager::Finalize();
	DirectXCommon::Finalize();
	WinApp::Finalize();
	DirectXCommon::ReleaseChack();

}
