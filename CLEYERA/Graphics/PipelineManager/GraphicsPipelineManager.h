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
	SPSOProperty Line;
	SPSOProperty Sprite;
};

class GraphicsPipelineManager
{
public:

	static GraphicsPipelineManager* GetInstance();

	static void Initialize();
    SPSO GetPso() { return pso; }

private:

	static void CreatePSO();
	static  SPSOProperty CreateShape(ComPtr<ID3D12Device>device,Commands command,SShaderMode shader);
	static SPSOProperty CreateLine(ComPtr<ID3D12Device>device, Commands commands, SShaderMode shader);
	static SPSOProperty CreateSprite(ComPtr<ID3D12Device>device, Commands commands, SShaderMode shader);


	SPSO pso = {};

};

