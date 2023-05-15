#pragma once
#include<windows.h>


class WinSetup
{
public:
	WinSetup();
	~WinSetup();

	/// <summary>
	/// �E�C���h�E�̕\���E������
	/// </summary>
	/// <param name="Width"></param>
	/// <param name="Height"></param>
	void Initialize(const int Width, const int Height);

	/// <summary>
	///�E�C���h�E���b�Z�[�W���� 
	/// </summary>
	/// <param name="msg"></param>
	void WinMSG(MSG& msg);

	HWND SetHwnd()
	{
		return hwnd;
	}
private:

	//�E�C���h�E�N���X
	WNDCLASS wc{};
	HWND hwnd;

};