#include "Cleyera.h"

Cleyera* Cleyera::GetInstance()
{
	static Cleyera instance;

	return &instance;
}

void Cleyera::Initialize()
{
	WinApp::Initialize();
	DirectXCommon::initialize();
}



void Cleyera::Finalize()
{
	DirectXCommon::Finalize();
	WinApp::Finalize();
}

void Cleyera::BeginFlame()
{
	DirectXCommon::BeginFlame();
}

void Cleyera::EndFlame()
{
	DirectXCommon::EndFlame();
}
