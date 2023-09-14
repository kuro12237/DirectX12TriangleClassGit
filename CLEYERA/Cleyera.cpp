#include "Cleyera.h"

Cleyera::~Cleyera()
{
}

Cleyera* Cleyera::GetInstance()
{
	static Cleyera instance;

	return &instance;
}

void Cleyera::Initialize()
{

	WinApp::Initialize();
	DirectXCommon::initialize();
	ShaderManager::Initialize();
	GraphicsPipelineManager::Initialize();
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
