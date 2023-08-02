#include"CLEYERA.h"


Cleyera::Cleyera()
{
}


Cleyera::~Cleyera()
{
}


void Cleyera::Initialize(const int32_t  kClientWidth, const int32_t  kClientHeight)
{
	//WinAppの初期化
	
	WinApp::Initialize(kClientWidth, kClientHeight);

	//
	////DirectXの初期化
	//
	
	///ファクトリー作成
	
	DirectXCommon::CreateDxgiFactory();

	///デバイス作成
	DirectXCommon::CreateDevice();

#ifdef _DEBUG

	DirectXSetup::debugErrorInfoQueue();

#endif // _DEBUG

	///コマンドリストの生成
	DirectXCommon::CreateCommands();

	//swapChain
	DirectXCommon::CreateSwapChain(kClientWidth, kClientHeight, WinApp::GetInstance()->GetHwnd());
	
	//rtvDescritor
	DirectXCommon::CreatertvDescritorHeap();
	
	//swapChainを引っ張る
	DirectXCommon::CreateSwapChainResorce();
	
	//RTVの設定と作成
	DirectXCommon::SettingandCreateRTV();

	//フェンスの生成
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
