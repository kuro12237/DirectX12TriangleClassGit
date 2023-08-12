#pragma once
#include"DirectXCommon.h"
#include"DirectXTex.h"
#include"d3dx12.h"

struct texResourceProperty
{
	D3D12_GPU_DESCRIPTOR_HANDLE SrvHandleGPU;
	ID3D12Resource* Resource;

};

struct DescriptorSize 
{
	uint32_t SRV;
	uint32_t RTV;
	uint32_t DSV;
};

class TexManager
{
public:
	TexManager();
	~TexManager();

	static void Initialize();

	static void Finalize();

	/// <summary>
	/// 実際に使う処理
	/// </summary>
	/// <param name="filePath"></param>
	/// <returns></returns>
	static texResourceProperty LoadTexture(const std::string& filePath);

	static texResourceProperty Release(texResourceProperty tex);


private:

	static ID3D12Resource* CreateTexResource(const DirectX::TexMetadata& metadata);

	static DirectX::ScratchImage CreateMipImage(const std::string& filePath);

	static void UploadTexData(ID3D12Resource* tex, const DirectX::ScratchImage& mipImage);

	static D3D12_CPU_DESCRIPTOR_HANDLE GetCPUDescriptorHandle(ID3D12DescriptorHeap* descripterHeap, uint32_t desiripterSize, uint32_t index);

	static D3D12_GPU_DESCRIPTOR_HANDLE GetGPUDescriptorHandle(ID3D12DescriptorHeap* descripterHeap, uint32_t desiripterSize, uint32_t index);


	/// <summary>
	///	リソースの設定 
	/// </summary>
	/// <param name="metadata"></param>
	/// <returns></returns>
	static D3D12_RESOURCE_DESC SettingResource(const DirectX::TexMetadata& metadata);

	/// <summary>
	/// ヒープの設定
	/// </summary>
	/// <returns></returns>
	static D3D12_HEAP_PROPERTIES SettingHeap();


	
};

