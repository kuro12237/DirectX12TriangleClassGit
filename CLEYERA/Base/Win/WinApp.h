#pragma once
#include"Pch.h"

class WinApp
{
public:
	
	static WinApp *GetInstance();

	static void Initialize();

	static void Msg(MSG& msg);

	static void Finalize();

#pragma region get
	static HWND GetHwnd() { return WinApp::GetInstance()->hwnd_; }
	static WNDCLASS GetWc() { return WinApp::GetInstance()->wc_; }


	static int32_t GetkCilientWidth() { return WinApp::GetInstance()->kWindowWidth; }
	static int32_t GetkCilientHeight() { return WinApp::GetInstance()->kWindowHeight; }
#pragma endregion

private:

	static LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

	static const int32_t kWindowWidth = 1280;
	static const int32_t kWindowHeight = 720;

	WNDCLASS wc_{};
	HWND hwnd_ = {};

	WinApp() = default;
	~WinApp() = default;
	WinApp(const WinApp&) = delete;
	const WinApp& operator=(const WinApp&) = delete;

};


