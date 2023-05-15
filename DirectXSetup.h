#pragma once
#include <Windows.h>
#include<d3d12.h>
#include<dxgi1_6.h>
#include<cassert>

#include <dxcapi.h>
#include<dxgidebug.h>

#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"dxgi.lib")

#pragma comment (lib,"dxguid.lib")
#pragma comment(lib,"dxcompiler.lib")
#include"Vector4.h"

#include"TrianglePos.h"

struct DirectX
{
	ID3D12Device* device = nullptr;

	IDxcUtils* dxcUtils = nullptr;
	IDxcCompiler3* dxcCompiler = nullptr;
	ID3DBlob* signaltureBlob = nullptr;
	ID3D10Blob* errorBlob = nullptr;
	D3D12_INPUT_ELEMENT_DESC inputElementDescs[1] = {};
	//�X���b�v�`�F�C���̍쐬
	IDXGISwapChain4* swapChain = nullptr;
	DXGI_SWAP_CHAIN_DESC1 swapChainDesc{};
	//�R�}���h�L���[�̍쐬
	ID3D12CommandQueue* commandQueue = nullptr;


	D3D12_COMMAND_QUEUE_DESC commandQueueDesc{};
	//�R�}���h�A���P�[�^�̍쐬
	ID3D12CommandAllocator* commandAllocator = nullptr;
	//�R�}���h���X�g���쐬����
	ID3D12GraphicsCommandList* commandList = nullptr;
	//�q�[�v�̍쐬
	ID3D12DescriptorHeap* rtvDescriptorheap = nullptr;
	D3D12_DESCRIPTOR_HEAP_DESC rtvDescDescriptorHeapDesc{};
	//RTV�̍쐬
	D3D12_RENDER_TARGET_VIEW_DESC rtvDesc{};
	///PSO
		//include���邽�߂̑Ή��ݒ�
	IDxcIncludeHandler* includeHandler = nullptr;

	//PSO�쐬
	D3D12_ROOT_SIGNATURE_DESC descriptionRootSignature{};
	//�o�C�i�������Ƃɍ쐬
	ID3D12RootSignature* rootsignature = nullptr;
	//InputLayout
	//D3D12_INPUT_ELEMENT_DESC inputElementDescs[1] = {};
	D3D12_INPUT_LAYOUT_DESC inputLayoutDesc = {};

	//BlendState�̐ݒ�
	D3D12_BLEND_DESC blendDesc{};
	//���_���\�[�X�̐ݒ�
	D3D12_RESOURCE_DESC vertexResourceDesc{};
	//RasiterzerState�̐ݒ�
	D3D12_RASTERIZER_DESC rasterizerDesc{};
	IDxcBlob* vertexShaderBlob;
	IDxcBlob* pixelShaderBlob;
	////PSO����
	D3D12_GRAPHICS_PIPELINE_STATE_DESC graphicsPipelineStateDesc{};

	ID3D12PipelineState* graphicsPipelineState = nullptr;
	//���_���\�[�X�p�̃q�[�v�̐ݒ�
	D3D12_HEAP_PROPERTIES uploadHEapProperties{};

	//���_���\�[�X�̐ݒ�
	//D3D12_RESOURCE_DESC vertexResourceDesc{};
	ID3D12Resource* vertexResource = nullptr;


	//���_�o�b�t�@�r���[���쐬����
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView{};
	D3D12_VIEWPORT viewport{};
	//�V�U�[��` 
	D3D12_RECT scissorRect{};

	ID3D12Fence* fence = nullptr;
	UINT64 fenceValue = 0;
	//���\�[�X�������Ă���
	ID3D12Resource* swapChainResources[2] = { nullptr };
	D3D12_CPU_DESCRIPTOR_HANDLE rtvStartHandle;
	D3D12_CPU_DESCRIPTOR_HANDLE rtvHandles[2];
	UINT backBufferIndex;

	//DXGI_SWAP_CHAIN_DESC1 swapChainDesc{};
};

class DirectXSetup
{
public:
	DirectXSetup();
	~DirectXSetup();
	/// <summary>
	/// ��ʂ̏�����
	/// </summary>
	/// <param name="Width"></param>
	/// <param name="Height"></param>
	void DisplayInitalize(const int Width, const int Height, HWND hwnd);

	void SettingPSO();
	void CreatePSO();
	void PolygonSetting(const int kClientWidth_, const int  kClientHeight_);

	void Draw(TrianglePos pos);

	/// <summary>
	/// DirectX�����̍Ō�ɏ���
	/// </summary>
	void DisplayFinish();

	/// <summary>
	/// �t���[���̍ŏ��̏���
	/// </summary>
	//void Beginframe();


	void BeginFlame();

	/// <summary>
	/// �t���[���̍Ō�̏���
	/// </summary>
	void EndFlame();


private:
	D3D12_RESOURCE_BARRIER barrier{};

	int kCientWidth_;
	int kCientHeight_;

	HRESULT hr;

	DirectX directX_;

	int Flag = 0;
	//float clearColor[4] = { 0.1f,0.25f,0.5f,1.0f };

	//int fenceValue = 0;
};