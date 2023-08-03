#pragma once
#include"DirectXCommon.h"

struct  ShaderMode
{
	IDxcBlob* vertexBlob;
	IDxcBlob* pixelBlob;
};

struct Shaders
{
	ShaderMode shape;
	ShaderMode sprite;

};


struct PSOProperty
{
	ID3D12PipelineState* GraphicsPipelineState = nullptr;
	ID3D12RootSignature* rootSignature = nullptr;
	ID3DBlob* signatureBlob = nullptr;
	ID3DBlob* errorBlob = nullptr;

};

struct PSO
{
	PSOProperty shape;

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
	static void Initialize();

	/// <summary>
	/// 解放処理
	/// </summary>
	static void Finalize();

	/// <summary>
	/// シェーダーをコンパイル
	/// </summary>
	static void ShaderCompile();

	/// <summary>
	/// パイプラインを作る
	/// </summary>
	static void PSOCreate();

	static PSO GetPSO() { return GraphicsPipeline::GetInstance()->pso_; }
	

private:

	/// <summary>
	/// シェーダーコンパイル関数
	/// </summary>
	/// <param name="filePath"></param>
	/// <param name="profile"></param>
	/// <param name="dxcUtils"></param>
	/// <param name="dxcCompiler"></param>
	/// <param name="includeHandler"></param>
	/// <returns></returns>
	static IDxcBlob* CompilerShaderFanc(
		const std::wstring& filePath,
		const wchar_t* profile
	);

	static void dxcCreate();

	static void DfIncludeHandlerSetting();


	static void ShapePSO();

	static void ShaderRelease(ShaderMode shader);

	static void PSORelease(PSOProperty pso);

	
	DXCProperty dxc = {};

	IDxcIncludeHandler* includeHandler = nullptr;
	
	Shaders shader_ = { nullptr,nullptr };
	

	PSO pso_;

};






