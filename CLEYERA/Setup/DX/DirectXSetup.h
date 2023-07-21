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




struct  RTV
{
	ID3D12DescriptorHeap* DescritorHeap;
	//D3D12_DESCRIPTOR_HEAP_DESC rtvDescritorHeapDesc{};


	//D3D12_RENDER_TARGET_VIEW_DESC rtvDesc{};
	D3D12_CPU_DESCRIPTOR_HANDLE rtvStartHandle;

	//RTV��2���̂Ńf�B�X�N���v�^��2�p��
	D3D12_CPU_DESCRIPTOR_HANDLE rtvHandles[2];
};
struct SwapChain
{
	IDXGISwapChain4* swapChain;
	ID3D12Resource* Resource[2];


};

struct Commands
{
	//�R�}���h�L���[
	ID3D12CommandQueue* Queue;
	D3D12_COMMAND_QUEUE_DESC QueueDesc{};
	//�R�}���h�A���[�P�[�^
	ID3D12CommandAllocator* Allocator;
	//�R�}���h���X�g
	ID3D12GraphicsCommandList* List;
};


class DirectXSetup
{
public:
	
	DirectXSetup();
	~DirectXSetup();




#pragma region Initialize�Ŏg���֐�

	/// <summary>
	/// DXGI�t�@�N�g���[�̍쐬
	/// </summary>
	void CreateDXGIFactorye();

	/// <summary>
	/// �f�o�C�X�̍쐬
	/// </summary>
	void CreateDevice();

	/// <summary>
	/// windows11�ł�DXGI�f�o�b�O���C���[��DX12�f�o�b�O���C���[�̑��݃o�O�ɂ��G���[�`�F�b�N
	/// </summary>
	void debugErrorInfoQueue();

	/// <summary>
    /// �R�}���h���X�g�̐���
    /// </summary>
	void CreateCommands();

	/// <summary>
    /// �X���b�v�`�F�[���̐ݒ�
    /// </summary>
	void CreateSwapChain(const int32_t Width, const int32_t Height, HWND hwnd_);


	ID3D12DescriptorHeap* CreateDescriptorHeap( D3D12_DESCRIPTOR_HEAP_TYPE heapType, UINT numDescriptors, bool shaderVisible);

	void CreatertvDescritorHeap();

	/// <summary>
	///  �X���b�v�`�F�[���̐���
	/// </summary>
	void CreateSwapChainResorce();

	/// <summary>
	/// RTV�̐���
	/// </summary>
	void SettingandCreateRTV();

	/// <summary>
	/// �t�F���X�̐���
	/// </summary>
	void CreateFence();


	ID3D12Device* GetDevice() { return device; }
	Commands GetCommands() { return commands; }

#pragma region ���[�v

	void BeginFlame();

	void ScissorViewCommand(const int32_t kClientWidth, const int32_t kClientHeight);

	/// <summary>
	/// �r���[�|�[�g�̐ݒ�
	/// </summary>
	/// <param name="kClientWidth"></param>
	/// <param name="kClientHeight"></param>
	/// <returns></returns>
	static D3D12_VIEWPORT viewportSetting(int32_t kClientWidth, int32_t kClientHeight);

	static D3D12_RECT scissorRectSetting(int32_t kClientWidth, int32_t kClientHeight);


	void EndFlame();

#pragma endregion

private:

	//�Ńo�b�N�p
	ID3D12Debug1* debugController = nullptr;


	//
	HRESULT hr;

	IDXGIFactory7* dxgiFactory = nullptr;
	IDXGIAdapter4* useAdapter = nullptr;
	ID3D12Device* device = nullptr;

	Commands commands;

	SwapChain swapChain;

	//DescriptorHeap
	RTV rtv;
	ID3D12DescriptorHeap* srvDescriptorHeap;

	//�t�F���X
	ID3D12Fence* fence = nullptr;
	uint64_t fenceValue;
	HANDLE fenceEvent;

	D3D12_RESOURCE_BARRIER barrier{};
};

