#include"WinSetup.h"
#include<string>
#include<format>


//�E�C���h�E�v���[�W���[
LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
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





std::wstring ConvertString(const std::string& str)
{
	if (str.empty())
	{
		return std::wstring();
	}

	auto sizeNeeded = MultiByteToWideChar(CP_UTF8, 0, reinterpret_cast<const char*>(&str[0]), static_cast<int>(str.size()), NULL, 0);
	if (sizeNeeded == 0)
	{
		return std::wstring();
	}
	std::wstring result(sizeNeeded, 0);
	MultiByteToWideChar(CP_UTF8, 0, reinterpret_cast<const char*>(&str[0]), static_cast<int>(str.size()), &result[0], sizeNeeded);
	return result;
}

std::string ConvertString(const std::wstring& str)
{
	if (str.empty())
	{
		return std::string();
	}

	auto sizeNeeded = WideCharToMultiByte(CP_UTF8, 0, str.data(), static_cast<int>(str.size()), NULL, 0, NULL, NULL);
	if (sizeNeeded == 0)
	{
		return std::string();
	}
	std::string result(sizeNeeded, 0);
	WideCharToMultiByte(CP_UTF8, 0, str.data(), static_cast<int>(str.size()), result.data(), sizeNeeded, NULL, NULL);
	return result;
}

//Log
void Log(const std::string& message)
{
	OutputDebugStringA(message.c_str());


}


void LogWinSetup(int Width, int Height)
{
	Log(ConvertString(std::format(L"CompliteWindow\nWinLog\nWidth:{}   Height:{}\n", Width, Height)));

}



WinSetup::WinSetup()
{


}

WinSetup::~WinSetup()
{
	CloseWindow(hwnd);
}


void WinSetup::Initialize(const int Width, const int Height)
{

	//�E�C���h�E�v���[�W���[
	wc.lpfnWndProc = WindowProc;

	//�E�C���h�E�N���X��
	wc.lpszClassName = L"directXClass";
	//�C���X�^���X
	wc.hInstance = GetModuleHandle(nullptr);
	//�J�[�\��
	wc.hCursor = LoadCursor(nullptr, IDC_ARROW);


	RegisterClass(&wc);

	const int32_t kCientWidth = Width;
	const int32_t kCientHeight = Height;
	//�N���C�A���g�̗̈�ݒ�
	RECT wrc = { 0,0,kCientWidth,kCientHeight };
	AdjustWindowRect(&wrc, WS_OVERLAPPEDWINDOW, false);



	//�E�C���h�E�̐���
	hwnd = CreateWindow(
		wc.lpszClassName,
		L"DirectX",
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		wrc.right - wrc.left,
		wrc.bottom - wrc.top,
		nullptr,
		nullptr,
		wc.hInstance,
		nullptr
	);
	ShowWindow(hwnd, SW_SHOW);

	LogWinSetup(Width, Height);

}

void WinSetup::WinMSG(MSG& msg)
{
	TranslateMessage(&msg);
	DispatchMessage(&msg);

}
