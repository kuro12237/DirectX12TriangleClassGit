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

	Input::DirectInputObjCreate();
	Input::KeyDeviceCreate();


	ImGuiManager::Initialize(WinApp::GetInstance(), DirectXSetup::GetInstance());

	TexManager::Initialize();

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
	
	//Tex描画のパイプライン
	Model::SpriteCreatePSO();

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