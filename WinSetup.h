#pragma once
#include<windows.h>


class WinSetup
{
public:
	WinSetup();
	~WinSetup();

	/// <summary>
	/// ウインドウの表示・初期化
	/// </summary>
	/// <param name="Width"></param>
	/// <param name="Height"></param>
	void Initialize(const int Width, const int Height);

	/// <summary>
	///ウインドウメッセージ処理 
	/// </summary>
	/// <param name="msg"></param>
	void WinMSG(MSG& msg);

	HWND SetHwnd()
	{
		return hwnd;
	}
private:

	//ウインドウクラス
	WNDCLASS wc{};
	HWND hwnd;

};