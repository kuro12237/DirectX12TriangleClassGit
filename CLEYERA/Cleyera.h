#pragma once
#include"Pch.h"
#include"WinApp.h"
#include"DirectXCommon.h"
#include"ShaderManager.h"
#include"GraphicsPipelineManager.h"
#include"Model.h"
#include"ImGuiManager.h"
class Cleyera
{
public:
	
	~Cleyera();

	static Cleyera* GetInstance();

	static void Initialize();
	static void Finalize();

	static void BeginFlame();
	static void EndFlame();
private:


};

