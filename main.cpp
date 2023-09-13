#include"Cleyera.h"

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	Cleyera::Initialize();


	while (WinApp::WinMsg())
	{
		Cleyera::BeginFlame();
	

		Cleyera::EndFlame();

	}
	Cleyera::Finalize();

	return 0;
}