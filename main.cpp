#include"Adapter.h"

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{


	Adapter* adapter_ = new Adapter;

	//Size
	const int32_t kClientWidth = 1280;
	const int32_t kClientHeight = 720;


	Vec4 left = { -1.0f,-0.5f,0.0f,1.0f };

	Vec4 top = { -0.5f,0.5f,0.0f,1.0f };

	Vec4 right= { 0.0f,-0.5f,0.0f,1.0f };



	Vec4 left2 = { 0.0f,-0.5f,0.0f,1.0f };

	Vec4 top2 = { 0.5f,0.5f,0.0f,1.0f };

	Vec4 right2 = { 1.0f,-0.5f,0.0f,1.0f };


	


	adapter_->Initialize(kClientWidth, kClientHeight);

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
		adapter_->BeginFlame(kClientWidth,kClientHeight);
		
		adapter_->TriangleDraw(top, left, right);

		adapter_->TriangleDraw(top2, left2, right2);

		adapter_->EndFlame();

		
	}



	return 0;
}