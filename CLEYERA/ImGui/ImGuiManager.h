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

	static void Initialize(WinApp* WinSetup_, DirectXCommon* DXSetup_);


	static void BeginFlame(DirectXCommon* DXSetup_);


	static void EndFlame(DirectXCommon* DXSetup_);

	static void Finalize();
private:

};
