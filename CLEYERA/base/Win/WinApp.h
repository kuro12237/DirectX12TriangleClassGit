#pragma once


#include<windows.h>

#include<string>
#include<format>

#include""
#include"../externals/imgui/imgui_impl_dx12.h"
#include"../externals/imgui/imgui_impl_win32.h"

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
	static void Initialize(const int32_t  kClientWidth, const int32_t  kClientHeight);

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

private:

	//ウインドウクラス
	WNDCLASS wc_{};

	HWND hwnd;
};