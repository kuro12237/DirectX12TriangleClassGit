#include"Adapter.h"


void Adapter::Initialize(const int32_t kClientWidth, const int32_t  kClientHeight)
{
	//windows
	WinSetup_->Initialize(kClientWidth,kClientHeight);


}

void Adapter::WinMSG(MSG &msg)
{

	WinSetup_->WinMSG(msg);

}

Adapter::Adapter()
{


}