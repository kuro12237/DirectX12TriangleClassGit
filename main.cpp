#include"Adapter.h"

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{

	Adapter* adapter_ = new Adapter;

	//Size
	const int32_t kClientWidth = 1280;
	const int32_t kClientHeight = 720;
    
	
	adapter_->Initialize(kClientWidth,kClientHeight);

	MSG msg{};

	//game
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