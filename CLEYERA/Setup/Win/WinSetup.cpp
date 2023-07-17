#include"WinSetup.h"



WindowsSetup::WindowsSetup()
{
}

WindowsSetup::~WindowsSetup()
{
}

LRESULT CALLBACK WindowsSetup::WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{


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


void WindowsSetup::Initialize(const int32_t  kClientWidth, const int32_t  kClientHeight)
{

	//�E�C���h�E�v���[�W���[
	wc_.lpfnWndProc = WindowProc;

	//�E�C���h�E�N���X��
	wc_.lpszClassName = L"CLEYERA";
	//�C���X�^���X
	wc_.hInstance = GetModuleHandle(nullptr);
	//�J�[�\��
	wc_.hCursor = LoadCursor(nullptr, IDC_ARROW);


	RegisterClass(&wc_);


	//�N���C�A���g�̗̈�ݒ�
	RECT wrc = { 0,0,kClientWidth,kClientHeight };
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



}

void WindowsSetup::WinMSG(MSG& msg)
{
	TranslateMessage(&msg);
	DispatchMessage(&msg);

}


void WindowsSetup::Deleate()
{
	CloseWindow(hwnd);


}