#pragma once
#include"Struct.h"

#include<windows.h>
#include<string>
#include <dxgidebug.h>
#include <dxcapi.h>
#include <d3d12.h>
#include <dxgi1_6.h>

#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"dxgi.lib")
#pragma comment(lib,"dxguid.lib")
#pragma comment(lib,"dxcompiler.lib")

#include<cassert>

#include "DirectXSetup.h"

class  ShapeDraw
{
public:
	 ShapeDraw();
	~ ShapeDraw();

	void DirectXSetDevice(ID3D12Device* device_);

	void DirectXSetCommands(Commands commands_);

	void CreateVertex(VertexProperty &vertex);

	void TriangleDraw(Vec4 top, Vec4 left, Vec4 right, VertexProperty &vertex);


private:

	ID3D12Device* device = nullptr;

	Commands commands;

	HRESULT hr;

};


