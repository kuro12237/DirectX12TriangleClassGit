
#include"WinSetup.h"
#include<string>
#include<format>
#include"Log.h"


LRESULT CALLBACK WindowsSetup::WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	//メッセージにおいて固有の動作を行う
	switch (msg)
	{
		//ウインドウが破棄
	case WM_DESTROY:
		//OSに対しアプリ終了を伝える
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hwnd, msg, wparam, lparam);
}





WindowsSetup::WindowsSetup()
{
}

void WindowsSetup::Initialize(const int Width, const int Height)
{

	//ウインドウプロージャー
	wc_.lpfnWndProc = WindowProc;

	//ウインドウクラス名
	wc_.lpszClassName = L"directXClass";
	//インスタンス
	wc_.hInstance = GetModuleHandle(nullptr);
	//カーソル
	wc_.hCursor = LoadCursor(nullptr, IDC_ARROW);


	RegisterClass(&wc_);

	const int32_t kCientWidth = Width;
	const int32_t kCientHeight = Height;
	//クライアントの領域設定
	RECT wrc = { 0,0,kCientWidth,kCientHeight };
	AdjustWindowRect(&wrc, WS_OVERLAPPEDWINDOW, false);



	//ウインドウの生成
	hwnd_ = CreateWindow(
		wc_.lpszClassName,
		L"DirectX",
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		wrc.right - wrc.left,
		wrc.bottom - wrc.top,
		nullptr,
		nullptr,
		wc_.hInstance,
		nullptr
	);
	ShowWindow(hwnd_, SW_SHOW);

	LogWinSetup(Width, Height);

}

void WindowsSetup::WinMSG(MSG& msg)
{
	TranslateMessage(&msg);
	DispatchMessage(&msg);

}


