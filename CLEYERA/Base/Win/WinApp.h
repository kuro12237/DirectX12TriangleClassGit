#pragma once
#include"Pch.h"

class WinApp
{
public:
	
	static WinApp *GetInstance();



	static HWND GetHwnd();

	static int32_t GetkCilientWidth() { return WinApp::GetInstance()->kCilientWidth; }
	static int32_t GetkCilientHeught() { return WinApp::GetInstance()->kCilientHeight; }

private:

	static const int32_t kCilientWidth = 720;
	static const int32_t kCilientHeight = 1280;

	HWND hwnd = {};

};


