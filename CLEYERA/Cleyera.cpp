#include "Cleyera.h"


Cleyera* Cleyera::GetInstance()
{
	static Cleyera instance;

	return &instance;
}

void Cleyera::Initialize()
{
	Cleyera::GetInstance();

	WinApp::Initialize();
	DirectXCommon::initialize();
	ShaderManager::Initialize();
	GraphicsPipelineManager::Initialize();
	TextureManager::Initialize();
	ImGuiManager::Initialize();
	DebugTools::Initialize();
}

void Cleyera::Finalize()
{
	TextureManager::Finalize();
	ImGuiManager::Finalize();
	DirectXCommon::Finalize();
	WinApp::Finalize();
}

void Cleyera::BeginFlame()
{
	DirectXCommon::BeginFlame();
	ImGuiManager::BeginFlame();
}

void Cleyera::EndFlame()
{
	ImGuiManager::EndFlame();
	DirectXCommon::EndFlame();
}
