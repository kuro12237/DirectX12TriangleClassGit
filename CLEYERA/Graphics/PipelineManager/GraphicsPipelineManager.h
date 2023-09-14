#pragma once
#include"LogManager.h"
#include"DirectXCommon.h"
#include"ShaderManager.h"


struct SPSOProperty
{
	ComPtr<ID3D12PipelineState> GraphicsPipelineState = nullptr;
	ComPtr<ID3D12RootSignature> rootSignature = nullptr;
	ComPtr<ID3DBlob> signatureBlob = nullptr;
	ComPtr<ID3DBlob> errorBlob = nullptr;

};

struct SPSO
{
	SPSOProperty shape;


};

class GraphicsPipelineManager
{
public:

	static GraphicsPipelineManager* GetInstance();

	static void Initialize();
    SPSO GetPso() { return pso; }

private:

	static void CreatePSO();
	static  SPSOProperty CreateShape(ComPtr<ID3D12Device>,Commands command,SShaderMode shader);




	SPSO pso = {};

};

