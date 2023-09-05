#pragma once
#include"Pch.h"
#include"WinApp.h"
#include"DirectXCommon.h"

class Cleyera
{
public:

	static Cleyera* GetInstance();

	static void Initialize();
	static void WinMSG(MSG msg);
	static void Finalize();

	static void BeginFlame();
	static void EndFlame();
private:

	//static D3DResourceLeakChecker leakCheck;
};

