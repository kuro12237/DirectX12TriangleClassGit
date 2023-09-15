#pragma once

#include<format>
#include<string>
#include<wrl.h>
#include<cassert>
#include<list>
#include<vector>

#include<Windows.h>

#include"imgui.h"
#include"imgui_impl_dx12.h"
#include"imgui_impl_win32.h"

#include <dxgidebug.h>
#include <dxcapi.h>
#include <d3d12.h>
#include <dxgi1_6.h>
#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"dxgi.lib")
#pragma comment(lib,"dxguid.lib")
#pragma comment(lib,"dxcompiler.lib")
#include"DirectXTex/DirectXTex.h"

using Microsoft::WRL::ComPtr;
using namespace std;

#include"Vector2.h"
#include"Vector3.h"
#include"Vector4.h"

#include"Matrix2x2.h"
#include"Matrix3x3.h"
#include"Matrix4x4.h"

