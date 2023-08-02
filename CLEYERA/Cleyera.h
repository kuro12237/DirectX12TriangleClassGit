#pragma once
#include<format>
#include"WinApp.h"
#include"DirectXCommon.h"
#include"ImGuiManager.h"



class Cleyera
{
public:
	Cleyera();
	~Cleyera();

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="kClientWidth"></param>
	/// <param name="kClientHeight"></param>
	static void Initialize(const int32_t  kClientWidth, const int32_t  kClientHeight);

	/// <summary>
	/// 解放
	/// </summary>
	static void Finalize();
#pragma region ループする必要がある処理


	/// <summary>
	/// メッセージをWinSetupに転送
	/// </summary>
	/// <param name="msg"></param>
	static void WinMSG(MSG msg);

	static void BeginFlame(const int32_t  kClientWidth, const int32_t  kClientHeight);

	static void EndFlame();
		  

private:



	

};

