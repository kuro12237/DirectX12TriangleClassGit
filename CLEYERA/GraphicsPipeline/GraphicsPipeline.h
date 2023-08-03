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
	/// �p�C�v���C���̏�����
	/// </summary>
	/// <param name="device"></param>
	/// <param name="commands"></param>
	static void Initialize();

	/// <summary>
	/// �������
	/// </summary>
	static void Finalize();

	/// <summary>
	/// �V�F�[�_�[���R���p�C��
	/// </summary>
	static void ShaderCompile();

	/// <summary>
	/// �p�C�v���C�������
	/// </summary>
	static void PSOCreate();

	static PSO GetPSO() { return GraphicsPipeline::GetInstance()->pso_; }
	

private:

	/// <summary>
	/// �V�F�[�_�[�R���p�C���֐�
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






