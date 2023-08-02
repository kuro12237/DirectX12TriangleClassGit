#pragma once
#include"DirectXCommon.h"


struct Shaders
{
	IDxcBlob* vertexBlob;
	IDxcBlob* pixelBlob;

};


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
class GraphicsPipeline
{
public:
	GraphicsPipeline();
	~GraphicsPipeline();


	static GraphicsPipeline* GetInstance();

	/// <summary>
	/// パイプラインの初期化
	/// </summary>
	/// <param name="device"></param>
	/// <param name="commands"></param>
	static void Initialize(ID3D12Device*device,Commands commands);


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
		const wchar_t* profile
	);


private:



	static void dxcCreate();

	static void DfIncludeHandlerSetting();

	ID3D12Device* device = nullptr;
	Commands commands;

	DXCProperty dxc;

	IDxcIncludeHandler* includeHandler = nullptr;
	

};






