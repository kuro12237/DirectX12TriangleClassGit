#pragma once
#include"DirectXSetup.h"
#include"WinApp.h"
#include"../externals/imgui/imgui.h"
#include"../externals/imgui/imgui_impl_dx12.h"
#include"../externals/imgui/imgui_impl_win32.h"

class ImGuiManager
{
public:
	ImGuiManager();
	~ImGuiManager();

	static void Initialize(WinApp* WinSetup_, DirectXSetup* DXSetup_);


	static void BeginFlame(DirectXSetup* DXSetup_);


	static void EndFlame(DirectXSetup* DXSetup_);

	static void Finalize();
private:

};
