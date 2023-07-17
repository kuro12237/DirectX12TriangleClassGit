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

#include"../LogManager/Log.h"


struct  RTV
{
	ID3D12DescriptorHeap* DescritorHeap;
	D3D12_RENDER_TARGET_VIEW_DESC rtvDesc{};

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


	static DirectXSetup* GetInstance();

#pragma region Initialize�Ŏg���֐�

	/// <summary>
	/// DXGI�t�@�N�g���[�̍쐬
	/// </summary>
	static void CreateDxgiFactory();

	/// <summary>
	/// �f�o�C�X�̍쐬
	/// </summary>
	static void CreateDevice();

	/// <summary>
	/// windows11�ł�DXGI�f�o�b�O���C���[��DX12�f�o�b�O���C���[�̑��݃o�O�ɂ��G���[�`�F�b�N
	/// </summary>
	static void debugErrorInfoQueue();

	/// <summary>
    /// �R�}���h���X�g�̐���
    /// </summary>
	static void CreateCommands();

	/// <summary>
    /// �X���b�v�`�F�[���̐ݒ�
    /// </summary>
	static void CreateSwapChain(const int32_t Width, const int32_t Height, HWND hwnd_);

	static ID3D12DescriptorHeap* CreateDescriptorHeap( D3D12_DESCRIPTOR_HEAP_TYPE heapType, UINT numDescriptors, bool shaderVisible);

	static void CreatertvDescritorHeap();

	/// <summary>
	///  �X���b�v�`�F�[���̐���
	/// </summary>
	static void CreateSwapChainResorce();

	/// <summary>
	/// RTV�̐���
	/// </summary>
	static void SettingandCreateRTV();

	/// <summary>
	/// �t�F���X�̐���
	/// </summary>
	static void CreateFence();





#pragma region �Q�b�^�[

	 ID3D12Device* GetDevice() { return device_; }
	 Commands GetCommands() { return commands; }

	 DXGI_SWAP_CHAIN_DESC1 GeSwapChainDesc() { return swapChainDesc; }

	 RTV GetRTV() { return rtv; }

	 ID3D12DescriptorHeap* GetSrvDescripterHeap() { return srvDescriptorHeap; }

#pragma endregion 
	
	 /// <summary>
     /// DirectX�̉��
     /// </summary>
	 static void Finalize();


	/// <summary>
	/// �����[�X�`�F�b�N
	/// </summary>
	static void ReleaseChack();



#pragma region ���[�v

	static void BeginFlame();

	static void ScissorViewCommand(const int32_t kClientWidth, const int32_t kClientHeight);

	static void EndFlame();


#pragma endregion

private:

#pragma region Finalize�ɏ�������


	static void FeanceReleace(ID3D12Fence* fence, HANDLE fenceEvent);

	static void DescripterRelease(RTV rtv, ID3D12DescriptorHeap* srvDescriptorHeap);

	static void SwapChainRelease(SwapChain swapChain);

	static void CommandsRelease(Commands commands);
#pragma endregion 

#pragma region ScissorViewCommand�ɏ�������
	/// <summary>
    /// �r���[�|�[�g�̐ݒ�
    /// </summary>
    /// <param name="kClientWidth"></param>
    /// <param name="kClientHeight"></param>
    /// <returns></returns>
	static D3D12_VIEWPORT viewportSetting(int32_t kClientWidth, int32_t kClientHeight);

	/// <summary>
	/// �V�U�[�̐ݒ�
	/// </summary>
	static D3D12_RECT scissorRectSetting(int32_t kClientWidth, int32_t kClientHeight);
#pragma endregion

	//�Ńo�b�N�p
	ID3D12Debug1* debugController_ = nullptr;

	IDXGIFactory7* dxgiFactory_ = nullptr;
	IDXGIAdapter4* useAdapter_ = nullptr;
    ID3D12Device* device_ = nullptr;

	Commands commands;

	SwapChain swapChain;
	DXGI_SWAP_CHAIN_DESC1 swapChainDesc{};

	//DescriptorHeap
	RTV rtv;
	ID3D12DescriptorHeap* srvDescriptorHeap;

	//�t�F���X
	ID3D12Fence* fence = nullptr;
	uint64_t fenceValue;
	HANDLE fenceEvent;

	D3D12_RESOURCE_BARRIER barrier{};

	
};

