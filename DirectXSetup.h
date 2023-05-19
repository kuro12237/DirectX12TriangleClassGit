#pragma once
#include<windows.h>
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


struct Commands
{
	//コマンドキュー
	ID3D12CommandQueue* Queue;
	D3D12_COMMAND_QUEUE_DESC QueueDesc{};
	//コマンドアローケータ
	ID3D12CommandAllocator* Allocator;
	//コマンドリスト
	ID3D12GraphicsCommandList* List;
};

struct SwapChain
{
	IDXGISwapChain4* swapChain;
	ID3D12Resource* Resource[2];


};

struct  RTV
{
	ID3D12DescriptorHeap* rtvDescritorHeap;
	D3D12_DESCRIPTOR_HEAP_DESC rtvDescritorHeapDesc{};


	D3D12_RENDER_TARGET_VIEW_DESC rtvDesc{};
	D3D12_CPU_DESCRIPTOR_HANDLE rtvStartHandle;

	//RTVを2つ作るのでディスクリプタを2つ用意
	D3D12_CPU_DESCRIPTOR_HANDLE rtvHandles[2];
};


//= rtvDescritorHeap->GetCPUDescriptorHandleForHeapStart();
class DirectXSetup
{
public:
	DirectXSetup();
	~DirectXSetup();

	void CreateDXGiFactory();

	void CreateDevice();

	void DebugErrorInfoQueue();

	void CreateCommands();

	void CreateSwapChain(const int32_t Width, const int32_t Height,	HWND hwnd_);

	void CreatertvDescritorHeap();

	void CreateSwapChainResorce();

	void SettingandCreateRTV();


	void BeginFlame();

	void EndFlame();
	
	void Deleate();

private:

	IDXGIFactory7* dxgiFactory = nullptr;
	IDXGIAdapter4* useAdapter = nullptr;
	ID3D12Device* device = nullptr;

	Commands commands;
	SwapChain swapChain;
	

	RTV rtv;
	HRESULT hr ;

	ID3D12Fence* fence = nullptr;
	uint64_t fenceValue;
	HANDLE fenceEvent;
};

