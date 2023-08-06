#pragma once
#include"DirectXCommon.h"
#include"DirectXTex.h"
#include"d3dx12.h"

struct texResourceProperty
{
	D3D12_GPU_DESCRIPTOR_HANDLE SrvHandleGPU;
	ID3D12Resource* Resource;

};

class TexManager
{
public:
	TexManager();
	~TexManager();

	static void Initialize();

	static void Finalize();




	/// <summary>
	/// ���ۂɎg������
	/// </summary>
	/// <param name="filePath"></param>
	/// <returns></returns>
	static texResourceProperty LoadTexture(const std::string& filePath);

	static texResourceProperty Release(texResourceProperty tex);


private:

	static ID3D12Resource* CreateTexResource(const DirectX::TexMetadata& metadata);

	static DirectX::ScratchImage CreateMipImage(const std::string& filePath);

	static void UploadTexData(ID3D12Resource* tex, const DirectX::ScratchImage& mipImage);



	/// <summary>
	///	���\�[�X�̐ݒ� 
	/// </summary>
	/// <param name="metadata"></param>
	/// <returns></returns>
	static D3D12_RESOURCE_DESC SettingResource(const DirectX::TexMetadata& metadata);

	/// <summary>
	/// �q�[�v�̐ݒ�
	/// </summary>
	/// <returns></returns>
	static D3D12_HEAP_PROPERTIES SettingHeap();


};
