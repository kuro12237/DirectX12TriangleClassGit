#pragma once
#include "WinSetup.h"
#include"DirectXSetup.h"
#include"TrianglePos.h"

class Adapter
{
public:



	Adapter();


	~Adapter();
	/// <summary>
	/// WinSetup��DirectX�̏�����
	/// </summary>
	void Initialize(const int Width_, const int Height_);
	/// <summary>
	/// �E�B���h�E���b�Z�[�W
	/// </summary>
	/// <param name="msg"></param>
	void WinMSG(MSG& msg);

	void BeginFlame();

	/// <summary>
	/// �t���[���̍Ō�̏���
	/// </summary>
	void EndFlame();

	void Draw(TrianglePos pos);



private:

	HWND hwnd_;

	WinSetup* WinSetup_ = new WinSetup();
	DirectXSetup* DXset = new DirectXSetup();
};


