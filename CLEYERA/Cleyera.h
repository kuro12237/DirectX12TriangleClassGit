#pragma once
#include"Pch.h"
#include"WinApp.h"
#include"DirectXCommon.h"

class Cleyera
{
public:

	static Cleyera* GetInstance();

	static void Initialize();
	static void Finalize();

	static void BeginFlame();
	static void EndFlame();
private:

	D3DResourceLeakChecker leakCheck;
};

