#include "WinApp.h"

WinApp *WinApp::GetInstance()
{
	static WinApp instance;
	return &instance;
}


void WinApp::Initialize()
{
	WNDCLASS wc_{};
	HWND hwnd;

	wc_.lpfnWndProc = WinApp::WindowProc;
	wc_.lpszClassName = L"CLEYERA";
	wc_.hInstance = GetModuleHandle(nullptr);
	wc_.hCursor = LoadCursor(nullptr, IDC_ARROW);
	RegisterClass(&wc_);

	RECT wrc = { 0,0,
	WinApp::GetInstance()->kWindowWidth,
	WinApp::GetInstance()->kWindowHeight };
	AdjustWindowRect(&wrc, WS_OVERLAPPEDWINDOW, false);

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
	WinApp::GetInstance()->hwnd_ = hwnd;

}

void WinApp::Msg(MSG& msg)
{
	TranslateMessage(&msg);
	DispatchMessage(&msg);
}

void WinApp::Finalize()
{
	CloseWindow(WinApp::GetInstance()->hwnd_);
}

LRESULT WinApp::WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{

	switch (msg)
	{
		//�E�C���h�E���j��
	case WM_DESTROY:
		//OS�ɑ΂��A�v���I����`����
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hwnd, msg, wparam, lparam);
}
