#include"Cleyera.h"


int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	const int32_t kClientWidth = 1280;
	const int32_t kClientHeight = 720;

	Cleyera::Initialize(kClientWidth, kClientHeight);

	MSG msg{};

	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			Cleyera::WinMSG(msg);
		}

		Cleyera::BeginFlame(kClientWidth, kClientHeight);




		Cleyera::EndFlame();

	}

	Cleyera::Finalize();


	return 0;
}
