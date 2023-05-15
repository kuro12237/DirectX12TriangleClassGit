#include"Adapter.h"
#include<cstdint>

#include<string>
#include<format>
#include<cassert>

#include"WinSetup.h"


void LogSetup(const std::string& message)
{
	OutputDebugStringA(message.c_str());


}

Adapter::Adapter()
{
	hwnd_ = nullptr;

}




Adapter::~Adapter()
{
	delete WinSetup_;
	delete DXset;
}

void Adapter::Initialize(const int Width_, const int Height_)
{
	LogSetup("SetupInitialze\n");
	//ウインドウの初期化
	WinSetup_->Initialize(Width_, Height_);
	LogSetup("DirectXInitialze");
	DXset->DisplayInitalize(Width_, Height_, WinSetup_->SetHwnd());

	LogSetup("SPO_setting\n");
	DXset->SettingPSO();
	DXset->CreatePSO();
	DXset->PolygonSetting(Width_, Height_);
	DXset->DisplayFinish();
	LogSetup("DirectX Display Complite!!\n");
}

void Adapter::WinMSG(MSG& msg)
{
	WinSetup_->WinMSG(msg);

}

void Adapter::BeginFlame()
{

	DXset->BeginFlame();
}

void Adapter::EndFlame()
{

	DXset->EndFlame();

}

void Adapter::Draw(TrianglePos pos)
{

	DXset->Draw(pos);

}
