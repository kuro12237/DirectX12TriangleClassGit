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
	
	DirectXSetup::CreateDxgiFactory();

	///デバイス作成
	DirectXSetup::CreateDevice();

#ifdef _DEBUG

	DirectXSetup::debugErrorInfoQueue();

#endif // _DEBUG

	///コマンドリストの生成
	DirectXSetup::CreateCommands();

	//swapChain
	DirectXSetup::CreateSwapChain(kClientWidth, kClientHeight, WinApp::GetInstance()->GetHwnd());
	
	//rtvDescritor
	DirectXSetup::CreatertvDescritorHeap();
	
	//swapChainを引っ張る
	DirectXSetup::CreateSwapChainResorce();
	
	//RTVの設定と作成
	DirectXSetup::SettingandCreateRTV();

	//フェンスの生成
	DirectXSetup::CreateFence();


	ImGuiManager::Initialize(WinApp::GetInstance(), DirectXSetup::GetInstance());

	///モデルの初期化

	//DXでつくったものを転送
	
	Model::SetDevice(DirectXSetup::GetInstance()->GetDevice());
	Model::SetCommands(DirectXSetup::GetInstance()->GetCommands());

	//DXCの初期化
	Model::dxcInitialize();
	//Compileするための対応処理

	Model::InitializeDfIncludeHandler();

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
	//カメラ
	//worldTransform.matWorld_ = Camera::worldViewProjectionMatrixFanc(worldTransform.matWorld_);

	Model::Draw(position,size,ColorCode,worldTransform, Resource);
}
