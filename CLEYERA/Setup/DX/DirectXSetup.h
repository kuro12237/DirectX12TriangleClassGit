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



	D3D12_CPU_DESCRIPTOR_HANDLE rtvStartHandle;

	D3D12_CPU_DESCRIPTOR_HANDLE rtvHandles[2];
};
struct SwapChain
{
	IDXGISwapChain4* swapChain;
	ID3D12Resource* Resource[2];


};

struct Commands
{

	ID3D12CommandQueue* Queue;
	D3D12_COMMAND_QUEUE_DESC QueueDesc{};

	ID3D12CommandAllocator* Allocator;

	ID3D12GraphicsCommandList* List;
};


class DirectXSetup
{
public:
	
	DirectXSetup();
	~DirectXSetup();




#pragma region Initialize�Ŏg���֐�

	/// <summary>
	/// ファクトリーの作成
	/// </summary>
	void CreateDXGIFactorye();

	/// <summary>
	///デバイス作成
	/// </summary>
	void CreateDevice();

	/// <summary>
	/// 
	/// </summary>
	void debugErrorInfoQueue();

	/// <summary>
    /// コマンド作成
    /// </summary>
	void CreateCommands();

	/// <summary>
    /// スワップチェイン作成
    /// </summary>
	void CreateSwapChain(const int32_t Width, const int32_t Height, HWND hwnd_);


	ID3D12DescriptorHeap* CreateDescriptorHeap( D3D12_DESCRIPTOR_HEAP_TYPE heapType, UINT numDescriptors, bool shaderVisible);

	void CreatertvDescritorHeap();

	/// <summary>
	///  �X���b�v�`�F�[���̐���
	/// </summary>
	void CreateSwapChainResorce();

	/// <summary>
	/// RTV
	/// </summary>
	void SettingandCreateRTV();

	/// <summary>
	/// フェンス作成
	/// </summary>
	void CreateFence();


	ID3D12Device* GetDevice() { return device; }
	Commands GetCommands() { return commands; }

#pragma region ���[�v

	void BeginFlame();

	void ScissorViewCommand(const int32_t kClientWidth, const int32_t kClientHeight);

	/// <summary>
	/// ビューポート
	/// </summary>
	/// <param name="kClientWidth"></param>
	/// <param name="kClientHeight"></param>
	/// <returns></returns>
	static D3D12_VIEWPORT viewportSetting(int32_t kClientWidth, int32_t kClientHeight);

	static D3D12_RECT scissorRectSetting(int32_t kClientWidth, int32_t kClientHeight);


	void EndFlame();

#pragma endregion

private:

	
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

	//フェンス
	ID3D12Fence* fence = nullptr;
	uint64_t fenceValue;
	HANDLE fenceEvent;

	D3D12_RESOURCE_BARRIER barrier{};
};

