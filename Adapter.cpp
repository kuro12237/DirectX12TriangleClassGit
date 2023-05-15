#include"Adapter.h"


void Adapter::Initialize(const int Width, const int Height)
{
	//windows
	WinSetup_->Initialize(Width,Height);


}

void Adapter::WinMSG(MSG &msg)
{

	WinSetup_->WinMSG(msg);

}

Adapter::Adapter()
{


}