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

	void BeginFlame();

	void EndFlame();

	void Deleate();

private:


	WindowsSetup* WinSetup_ = new WindowsSetup();
	DirectXSetup* DXSetup_ = new DirectXSetup();

	
};



