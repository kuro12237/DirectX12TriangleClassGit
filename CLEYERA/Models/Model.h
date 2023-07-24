#pragma once

#include<windows.h>
#include<format>
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


#include"../Setup/DX/DirectXSetup.h"

#include"../Vector/Vector4.h"
#include"../Vector/Vector3.h"

#include"../Matrix/Matrix4x4.h"
#include"../Matrix/MatrixTransform.h"
struct PSOProperty
{
	ID3D12PipelineState* GraphicsPipelineState = nullptr;
	ID3D12RootSignature* rootSignature = nullptr;
	ID3DBlob* signatureBlob = nullptr;
	ID3DBlob* errorBlob = nullptr;

};

struct DXCProperty
{
	IDxcUtils* Utils;
	IDxcCompiler3* Compiler;

};


struct Shaders
{
	IDxcBlob* vertexBlob;
	IDxcBlob* pixelBlob;
};


struct  Position
{
	Vector3 top;
	Vector3 left;
	Vector3 right;

};

struct ResourcePeroperty
{
	
   D3D12_VERTEX_BUFFER_VIEW BufferView;
   ID3D12Resource* Vertex;
   ID3D12Resource* Material;
   ID3D12Resource* wvpResource;
	

};

class Model
{
public:
	Model();
	~Model();

#pragma region 初期化
	void Initialize();

	void SetDevice(ID3D12Device* device_);
	void SetCommands(Commands command);

	void dxcInitialize();

	void InitializeDfIncludeHandler();


	/// <summary>
    /// シェーダーコンパイル関数
    /// </summary>
    /// <param name="filePath"></param>
    /// <param name="profile"></param>
    /// <param name="dxcUtils"></param>
    /// <param name="dxcCompiler"></param>
    /// <param name="includeHandler"></param>
    /// <returns></returns>
	static IDxcBlob* CompilerShader(
		const std::wstring& filePath,
		const wchar_t* profile,
		IDxcUtils* dxcUtils,
		IDxcCompiler3* dxcCompiler,
		IDxcIncludeHandler* includeHandler);

	/// <summary>
	/// 実際にコンパイルする
	/// </summary>
	void CompileShaders();

	/// <summary>
	/// 色の図形のPSO生成
	/// </summary>
	void ShapeCreatePSO();

	/// <summary>
	/// コンパイルしたシェーダーのリリース
	/// </summary>
	void ShaderRelease();




#pragma endregion

#pragma region 三角形

	/// <summary>
	/// Resourceを作成
	/// </summary>
	/// <param name="device"></param>
	/// <param name="sizeInbyte"></param>
	/// <returns></returns>
	static ID3D12Resource* CreateBufferResource(ID3D12Device* device, size_t sizeInbyte);
	
	/// <summary>
	/// BufferViewを作成
	/// </summary>
	/// <param name="sizeInbyte"></param>
	/// <param name="Resource"></param>
	/// <returns></returns>
	static D3D12_VERTEX_BUFFER_VIEW CreateBufferView(size_t sizeInbyte, ID3D12Resource* Resource);

	/// <summary>
	/// Resource生成
	/// </summary>
	/// <returns></returns>
	ResourcePeroperty  CreateResource();

	/// <summary>
	/// 色変換
	/// </summary>
	/// <param name="color"></param>
	/// <returns></returns>
	Vector4 ColorAdapter(unsigned int color);

	/// <summary>
	/// 表示
	/// </summary>
	/// <param name="position"></param>
	/// <param name="Color"></param>
	/// <param name="Resource"></param>
	void ShapeDraw(Vector3 position, unsigned int Color, Matrix4x4 matrix,ResourcePeroperty Resource);

	/// <summary>
	/// Commands
	/// </summary>
	/// <param name="commands"></param>
	static void ShapeDrawCommands(Commands commands, ResourcePeroperty Resource,PSOProperty Shape);


#pragma endregion




private:

	ID3D12Device* device=nullptr;
	Commands commands;

	DXCProperty dxc;
	IDxcIncludeHandler* includeHandler = nullptr;
	Shaders shader;

	PSOProperty Shape;

	
};


