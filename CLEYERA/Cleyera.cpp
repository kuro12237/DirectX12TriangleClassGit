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

	//WinSetupの初期化
	

	WinSetup->Initialize(kClientWidth, kClientHeight);

	//
	////DirectXの初期化
	//

	///ファクトリー作成
	DXSetup->CreateDXGIFactorye();

	///デバイス作成
	DXSetup->CreateDevice();

#ifdef _DEBUG

	DXSetup->debugErrorInfoQueue();

#endif // _DEBUG

	///コマンドリストの生成
	DXSetup->CreateCommands();

	//swapChain
	DXSetup->CreateSwapChain(kClientWidth, kClientHeight, WinSetup->GetHwnd());
	//rtvDescritor
	DXSetup->CreatertvDescritorHeap();
	//swapChainを引っ張る
	DXSetup->CreateSwapChainResorce();
	//RTVの設定と作成
	DXSetup->SettingandCreateRTV();
	//フェンスの生成
	DXSetup->CreateFence();

	///モデルの初期化

	//DXでつくったものを転送
	model->SetDevice(DXSetup->GetDevice());
	model->SetCommands(DXSetup->GetCommands());

	//DXCの初期化
	model->dxcInitialize();
	//Compileするための対応処理
	model->InitializeDfIncludeHandler();
	
	///シェーダーコンパイル処理
	model->CompileShaders();

	//図形描画のパイプライン
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



