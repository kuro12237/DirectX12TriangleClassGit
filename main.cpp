#include"Adapter.h"

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{


	Adapter* adapter_ = new Adapter;

	adapter_->Initialize(1280, 720);

	MSG msg{};


	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			adapter_->WinMSG(msg);


		}
		else
		{

		}


	}



	return 0;
}