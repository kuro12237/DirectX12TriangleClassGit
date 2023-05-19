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

	//終了

}

void Adapter::WinMSG(MSG &msg)
{

	WinSetup_->WinMSG(msg);

}

void Adapter::BeginFlame()
{
	DXSetup_->BeginFlame();
}

void Adapter::EndFlame()
{
	DXSetup_->EndFlame();
}

void Adapter::Deleate()
{
	DXSetup_->Deleate();
}

Adapter::Adapter()
{


}