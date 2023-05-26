#include"Adapter.h"


void Adapter::Initialize(const int32_t Width, const int32_t Height)
{
	//windows
	WinSetup_->Initialize(Width,Height);
	//終了

	//DirectX
	DXSetup_->CreateDXGiFactory();

	DXSetup_->CreateDevice();

#ifdef _DEBUG
	//エラー、警告
	DXSetup_->DebugErrorInfoQueue();

#endif // _DEBUG
	
	//CommandList
	DXSetup_->CreateCommands();
	//swapChain
	DXSetup_->CreateSwapChain(Width,Height,WinSetup_->SetHwnd());
	//rtvDescritor
	DXSetup_->CreatertvDescritorHeap();
	//swapChainを引っ張る
	DXSetup_->CreateSwapChainResorce();
	//RTVの設定と作成
	DXSetup_->SettingandCreateRTV();
	//DXCの初期化
	DXSetup_->DXCInitialize();
	//PSOの生成
	DXSetup_->CreatePSO();



	//終了

}

void Adapter::WinMSG(MSG &msg)
{

	WinSetup_->WinMSG(msg);

}

void Adapter::BeginFlame(const int32_t kClientWidth, const int32_t kClientHeight)
{
	
	DXSetup_->BeginFlame(kClientWidth,kClientHeight);
}

void Adapter::EndFlame()
{
	DXSetup_->EndFlame();
}

void Adapter::TriangleDraw(Vec4 top, Vec4 left, Vec4 right)
{
	DXSetup_->CreateVecrtexResource();
	DXSetup_->Draw(top, left, right);
}

void Adapter::Deleate()
{
	DXSetup_->Deleate();
	WinSetup_->Deleate();
}

Adapter::Adapter()
{
	
}

Adapter::~Adapter()
{
	DXSetup_->~DirectXSetup();
	WinSetup_->~WindowsSetup();
}
