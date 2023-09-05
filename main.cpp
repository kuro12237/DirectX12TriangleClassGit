#include"Cleyera.h"

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{

	Cleyera::Initialize();
	MSG msg{};

	while (msg.message!=WM_QUIT)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			Cleyera::WinMSG(msg);
		}

		Cleyera::BeginFlame();

		Cleyera::EndFlame();

	}
	Cleyera::Finalize();

	return 0;
}