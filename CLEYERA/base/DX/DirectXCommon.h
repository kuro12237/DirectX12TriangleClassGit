#pragma once
#include"Pch.h"
#include"WinApp.h"
#include"LogManager.h"

struct D3DResourceLeakChecker{
	~D3DResourceLeakChecker() {
		ComPtr<IDXGIDebug1>debug;
		if (SUCCEEDED(DXGIGetDebugInterface1(0, IID_PPV_ARGS(&debug))))
		{
			debug->ReportLiveObjects(DXGI_DEBUG_ALL, DXGI_DEBUG_RLO_ALL);
			debug->ReportLiveObjects(DXGI_DEBUG_APP, DXGI_DEBUG_RLO_ALL);
			debug->ReportLiveObjects(DXGI_DEBUG_D3D12, DXGI_DEBUG_RLO_ALL);
		}
	}
};

struct Commands {
	ComPtr<ID3D12CommandAllocator>m_pAllocator;
	ComPtr<ID3D12GraphicsCommandList>m_pList;
	ComPtr<ID3D12CommandQueue>m_pQueue;
	D3D12_COMMAND_QUEUE_DESC QueueDesc{};
};

struct SwapChain{
	ComPtr<IDXGISwapChain4> m_pSwapChain;
	ComPtr<ID3D12Resource> m_pResource[2];
	DXGI_SWAP_CHAIN_DESC1 swapChainDesc{};
};
struct  RTV{
	ComPtr<ID3D12DescriptorHeap> m_pDescritorHeap;
	D3D12_RENDER_TARGET_VIEW_DESC rtvDesc{};
	D3D12_CPU_DESCRIPTOR_HANDLE rtvStartHandle;
	D3D12_CPU_DESCRIPTOR_HANDLE rtvHandles[2];
};

class DirectXCommon{
public:
	
	static DirectXCommon* GetInstance();

	static void initialize();
	static void Finalize();

	static void BeginFlame();
	static void EndFlame();

private:

	static ComPtr<ID3D12DescriptorHeap> CreateDescripterHeap(D3D12_DESCRIPTOR_HEAP_TYPE heapType, UINT numDescriptors, bool shaderVisible);
	static ComPtr<ID3D12Resource> CreateDepthStencilTextureResource();
	static void CreateFactory();
	static void CreateDevice();
	static void CreateCommands();
	static void CreateSwapChain();
	static void CreateDescritorHeap();
	static void CreateSwapChainResource();
	static void CreateRTV();
	static void CreateFence();

	//DXGI+ID3D12‚ÍŠî–{Comptr‚É•Ï‚¦‚é

    ComPtr<IDXGIFactory7> m_pDxgiFactory_ = nullptr;
	ComPtr<IDXGIAdapter4> m_pUseAdapter_ = nullptr;
	ComPtr<ID3D12Device>m_pDevice_ = nullptr;
	ComPtr<ID3D12Debug1> m_pDebugController = nullptr;
	Commands commands = {};
	SwapChain swapChain = {};
	RTV rtv = {};
	ComPtr<ID3D12DescriptorHeap> m_pSrvDescriptorHeap;
	ComPtr<ID3D12DescriptorHeap> m_pDsvDescripterHeap;
	ComPtr<ID3D12Resource> m_pDepthResource = nullptr;
	ComPtr<ID3D12Fence> m_pFence_ = nullptr;
	uint64_t fenceValue = {};
	HANDLE fenceEvent = {};
	D3D12_RESOURCE_BARRIER barrier{};


	DirectXCommon() = default;
	~DirectXCommon() = default;
	DirectXCommon(const DirectXCommon&) = delete;
	const DirectXCommon& operator=(const DirectXCommon&) = delete;

};

