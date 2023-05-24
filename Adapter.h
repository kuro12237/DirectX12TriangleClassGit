#pragma once
#include"WinSetup.h"
#include<string>
#include<format>
#include "DirectXSetup.h"

class Adapter
{
public:
	Adapter();
	~Adapter();

	void Initialize(const int32_t Width, const int32_t Height);

	void WinMSG( MSG &msg);

	void BeginFlame(const int32_t kClientWidth, const int32_t kClientHeight);

	void EndFlame();

	void Triangle(Vec4 top, Vec4 left, Vec4 right);
	void Deleate();

private:


	WindowsSetup* WinSetup_ = new WindowsSetup();
	DirectXSetup* DXSetup_ = new DirectXSetup();

	
};



