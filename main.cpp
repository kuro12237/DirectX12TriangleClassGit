#include"Adapter.h"

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	Adapter* adapter_ = new Adapter();

	adapter_->Initialize(1280, 720);





	return 0;
}