#pragma once


#include<windows.h>

#include<string>
#include<format>

#include"imgui.h"
#include"imgui_impl_dx12.h"
#include"imgui_impl_win32.h"

//#include"Log.h"

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);



class WinApp
{
public:
	WinApp();
	~WinApp();

	static WinApp* GetInstance();

	/// <summary>
	/// ウインドウの表示・初期化
	/// </summary>
	/// <param name="Width"></param>
	/// <param name="Height"></param>
	static void Initialize();

	/// <summary>
	///ウインドウメッセージ処理 
	/// </summary>
	/// <param name="msg"></param>
	static void Msg(MSG& msg);

	/// <summary>
	/// Close
	/// </summary>
	static void Finalize();


	static LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

	HWND GetHwnd()
	{
		return hwnd;
	}

	WNDCLASS GetWc()
	{
		return wc_;
	}

	static const int32_t GetkClinentWidth() 
	{ return WinApp::GetInstance()->kClinentWidth; }

	static const int32_t GetkClinentHeight() 
	{ return WinApp::GetInstance()->kClinentHeight; }

private:

	//ウインドウクラス
	WNDCLASS wc_{};

	HWND hwnd;

	static const int32_t kClinentWidth = 1280;
	static const int32_t kClinentHeight = 720;


};