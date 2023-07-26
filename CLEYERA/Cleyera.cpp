#include"CLEYERA.h"

Cleyera::Cleyera(){}

Cleyera::~Cleyera(){}

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


	ImGuiManager::Initialize(WinApp::GetInstance(), DirectXCommon::GetInstance());

	///モデルの初期化

	//DXでつくったものを転送
	
	Model::SetDevice(DirectXCommon::GetInstance()->GetDevice());
	Model::SetCommands(DirectXCommon::GetInstance()->GetCommands());

	//DXCの初期化
	Model::dxcInitialize();
	//Compileするための対応処理

	Model::DefaultIncludeHandlerInitialize();

	///シェーダーコンパイル処理
	Model::CompileShaders();
	
	//図形描画のパイプライン
	Model::ShapeCreatePSO();
	
	
	//コンパイルしたシェーダーの解放
	Model::ShaderRelease();
	
	//カメラの初期化
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
	//カメラ
	worldTransform.matWorld_ = Camera::worldViewProjectionMatrixFanc(worldTransform.matWorld_);
	position, size, ColorCode, Resource;
	//Model::Draw(position,size,ColorCode,worldTransform, Resource);
}
