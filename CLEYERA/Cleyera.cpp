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

	DirectXCommon::debugErrorInfoQueue();

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
	

	//IGuiの初期化
	ImGuiManager::Initialize(WinApp::GetInstance(), DirectXCommon::GetInstance());


	//Pipelineの初期化
	GraphicsPipeline::Initialize();

	//シェーダーをコンパイル
	GraphicsPipeline::ShaderCompile();

	//Pipelineの作成
	GraphicsPipeline::PSOCreate();

	//Cameraの初期化
	Camera::Initialize(kClientWidth, kClientHeight);



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

	GraphicsPipeline::Finalize();

	ImGuiManager::Finalize();

	DirectXCommon::Finalize();
	WinApp::Finalize();

	DirectXCommon::ReleaseChack();

}
