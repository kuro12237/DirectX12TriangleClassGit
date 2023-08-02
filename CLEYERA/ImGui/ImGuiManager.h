#pragma once
#include"DirectXCommon.h"
#include"WinApp.h"
#include"imgui.h"
#include"imgui_impl_dx12.h"
#include"imgui_impl_win32.h"

class ImGuiManager
{
public:
	ImGuiManager();
	~ImGuiManager();

	static void Initialize(WinApp* WinApp_, DirectXCommon* DXCommon_);


	static void BeginFlame(DirectXCommon* DXCommon_);


	static void EndFlame(DirectXCommon* DXCommn_);

	static void Finalize();
private:

};
