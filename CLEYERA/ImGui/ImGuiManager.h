#pragma once
#include"DirectXSetup.h"
#include"WinApp.h"
#include"imgui.h"
#include"imgui_impl_dx12.h"
#include"imgui_impl_win32.h"

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
