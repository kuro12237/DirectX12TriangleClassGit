#pragma once
#include "WinSetup.h"
#include"DirectXSetup.h"
#include"TrianglePos.h"

class Adapter
{
public:



	Adapter();


	~Adapter();
	/// <summary>
	/// WinSetupとDirectXの初期化
	/// </summary>
	void Initialize(const int Width_, const int Height_);
	/// <summary>
	/// ウィンドウメッセージ
	/// </summary>
	/// <param name="msg"></param>
	void WinMSG(MSG& msg);

	void BeginFlame();

	/// <summary>
	/// フレームの最後の処理
	/// </summary>
	void EndFlame();

	void Draw(TrianglePos pos);



private:

	HWND hwnd_;

	WinSetup* WinSetup_ = new WinSetup();
	DirectXSetup* DXset = new DirectXSetup();
};


