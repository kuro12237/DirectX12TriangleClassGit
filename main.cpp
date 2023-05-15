
#include "Adapter.h"


int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	Adapter* adapter_ = new Adapter();

	const int kClientWidth = 1280;
	const int kClientHeight = 720;


	adapter_->Initialize(kClientWidth, kClientWidth);

	MSG msg{};

	TrianglePos pos[10];
	pos[0] =
	{
		{-0.3f,-0.5f,0.0f,1.0f},//left
		{-0.5f,0.3f,0.0f,1.0f},//top
		{-0.7f,-0.5f,0.0f,1.0f}//right
	};
	pos[1] =
	{
			{1.0f,-0.5f,0.0f,1.0f},//left
			{0.7f,0.25f,0.0f,1.0f},//top
			{0.5f,-0.5f,0.0f,1.0f}//right
	};
	pos[2] =
	{
			{1.0f,-1.9f,0.0f,1.0f},//left
			{1.0f,0.25f,0.0f,1.0f},//top
			{0.5f,-1.9f,0.0f,1.0f}//right
	};
	pos[3] =
	{
		{0.3f,1.0f,0.0f,1.0f},//left
		{0.3f,0.0f,0.0f,1.0f},//top
		{0.7f,1.0f,0.0f,1.0f}//right
	};
	pos[4] =
	{
		{0.3f,1.0f,0.0f,1.0f},//left
		{0.7f,0.0f,0.0f,1.0f},//top
		{0.7f,1.0f,0.0f,1.0f}//right
	};
	pos[5] =
	{
		{0.3f,0.0f,0.0f,1.0f},//left
		{0.3f,-1.0f,0.0f,1.0f},//top
		{0.7f,0.0f,0.0f,1.0f}//right
	};
	pos[6] =
	{
		{0.3f,0.0f,0.0f,1.0f},//left
		{0.7f,-1.0f,0.0f,1.0f},//top
		{0.7f,0.0f,0.0f,1.0f}//right
	};
	//
	pos[7] =
	{
		{-0.3f,-1.0f,0.0f,1.0f},//left
		{-0.3f,-0.0f,0.0f,1.0f},//top
		{-0.7f,-1.0f,0.0f,1.0f}//right
	};
	pos[8] =
	{
		{-0.3f,-1.0f,0.0f,1.0f},//left
		{-0.7f,0.2f,0.0f,1.0f},//top
		{-0.7f,-1.0f,0.0f,1.0f}//right
	};
	pos[9] =
	{
		{-0.5f,-1.0f,0.0f,1.0f},//left
		{-0.9f,0.2f,0.0f,1.0f},//top
		{-0.9f,-1.0f,0.0f,1.0f}//right
	};




	while (msg.message != WM_QUIT)
	{
		adapter_->BeginFlame();

		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))//ƒQ[ƒ€ˆ—
		{
			adapter_->WinMSG(msg);


		}
		else
		{

		}


		for (int i = 0; i < 10; i++)
		{
			adapter_->Draw(pos[i]);


		}
		adapter_->EndFlame();

	}

	adapter_->~Adapter();



	//return 0;
}
