#pragma once
#include<windows.h>
#include<string>
#include<format>

class WindowsSetup
{
public:
	WindowsSetup();
	~WindowsSetup();

	/// <summary>
	/// window:Show:Initialize
	/// </summary>
	/// <param name="Width"></param>
	/// <param name="Height"></param>
	void Initialize(const int32_t Width, const int32_t Height);

	/// <summary>
	///WinMsg
	/// </summary>
	/// <param name="msg"></param>
	void WinMSG(MSG& msg);

	HWND SetHwnd()
	{
		return hwnd_;
	}


	static LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

private:

	//WindowClass
	WNDCLASS wc_{};
	HWND hwnd_;

};