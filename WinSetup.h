#pragma once
#include<windows.h>


class WindowsSetup
{
public:
	WindowsSetup();
	~WindowsSetup();

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

	
	static LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

private:

	//ウインドウクラス
	WNDCLASS wc_{};
	HWND hwnd_;

};