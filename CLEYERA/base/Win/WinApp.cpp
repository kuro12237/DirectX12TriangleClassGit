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
	//���b�Z�[�W�ɂ����ČŗL�̓�����s��
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


void WinApp::Initialize()
{
	WNDCLASS wc_{};

	HWND hwnd;
	//�E�C���h�E�v���[�W���[
	//wc_.lpfnWndProc = WindowProc;
	wc_.lpfnWndProc = WinApp::WindowProc;

	//�E�C���h�E�N���X��
	//wc_.lpszClassName = L"CLEYERA";
    wc_.lpszClassName = L"CLEYERA";

	//�C���X�^���X
	wc_.hInstance = GetModuleHandle(nullptr);
	//wc_.hInstance = GetModuleHandle(nullptr);
	//�J�[�\��
	wc_.hCursor = LoadCursor(nullptr, IDC_ARROW);


	RegisterClass(&wc_);


	//�N���C�A���g�̗̈�ݒ�
	RECT wrc = { 0,0,
		WinApp::GetInstance()->kClinentWidth,
	WinApp::GetInstance()->kClinentHeight};
	AdjustWindowRect(&wrc, WS_OVERLAPPEDWINDOW, false);



	//�E�C���h�E�̐���
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