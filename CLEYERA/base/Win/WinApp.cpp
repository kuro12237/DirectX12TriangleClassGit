#include"WinApp.h"



WinApp::WinApp()
{
}

WinApp::~WinApp()
{
}

WinApp* WinApp::GetInstance()
{
	static WinApp instance;
	return &instance;

}

LRESULT CALLBACK WinApp::WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{

	if (ImGui_ImplWin32_WndProcHandler(hwnd, msg, wparam, lparam))
	{
		return true;
	}
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


void WinApp::Initialize()
{
	WNDCLASS wc_{};

	HWND hwnd;
	//ウインドウプロージャー
	//wc_.lpfnWndProc = WindowProc;
	wc_.lpfnWndProc = WinApp::WindowProc;

	//ウインドウクラス名
	//wc_.lpszClassName = L"CLEYERA";
    wc_.lpszClassName = L"CLEYERA";

	//インスタンス
	wc_.hInstance = GetModuleHandle(nullptr);
	//wc_.hInstance = GetModuleHandle(nullptr);
	//カーソル
	wc_.hCursor = LoadCursor(nullptr, IDC_ARROW);


	RegisterClass(&wc_);


	//クライアントの領域設定
	RECT wrc = { 0,0,
		WinApp::GetInstance()->kClinentWidth,
	WinApp::GetInstance()->kClinentHeight};
	AdjustWindowRect(&wrc, WS_OVERLAPPEDWINDOW, false);



	//ウインドウの生成
	hwnd = CreateWindow(
		wc_.lpszClassName,
		L"CLEYERA",
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
	ShowWindow(hwnd, SW_SHOW);
	WinApp::GetInstance()->wc_ = wc_;
	WinApp::GetInstance()->hwnd = hwnd;


}

void WinApp::Msg(MSG& msg)
{
	TranslateMessage(&msg);
	DispatchMessage(&msg);

}


void WinApp::Finalize()
{
	CloseWindow(WinApp::GetInstance()->hwnd);


}