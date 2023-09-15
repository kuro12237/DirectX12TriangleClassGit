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
	ImGuiManager::Initialize();
}

void Cleyera::Finalize()
{
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
