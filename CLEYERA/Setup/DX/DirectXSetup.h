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

	//RTVを2つ作るのでディスクリプタを2つ用意
	D3D12_CPU_DESCRIPTOR_HANDLE rtvHandles[2];
};
struct SwapChain
{
	IDXGISwapChain4* swapChain;
	ID3D12Resource* Resource[2];


};

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


class DirectXSetup
{
public:
	
	DirectXSetup();
	~DirectXSetup();




#pragma region Initializeで使う関数

	/// <summary>
	/// DXGIファクトリーの作成
	/// </summary>
	void CreateDXGIFactorye();

	/// <summary>
	/// デバイスの作成
	/// </summary>
	void CreateDevice();

	/// <summary>
	/// windows11でのDXGIデバッグレイヤーとDX12デバッグレイヤーの相互バグによるエラーチェック
	/// </summary>
	void debugErrorInfoQueue();

	/// <summary>
    /// コマンドリストの生成
    /// </summary>
	void CreateCommands();

	/// <summary>
    /// スワップチェーンの設定
    /// </summary>
	void CreateSwapChain(const int32_t Width, const int32_t Height, HWND hwnd_);


	ID3D12DescriptorHeap* CreateDescriptorHeap( D3D12_DESCRIPTOR_HEAP_TYPE heapType, UINT numDescriptors, bool shaderVisible);

	void CreatertvDescritorHeap();

	/// <summary>
	///  スワップチェーンの生成
	/// </summary>
	void CreateSwapChainResorce();

	/// <summary>
	/// RTVの生成
	/// </summary>
	void SettingandCreateRTV();

	/// <summary>
	/// フェンスの生成
	/// </summary>
	void CreateFence();


	ID3D12Device* GetDevice() { return device; }
	Commands GetCommands() { return commands; }

#pragma region ループ

	void BeginFlame();

	void ScissorViewCommand(const int32_t kClientWidth, const int32_t kClientHeight);

	/// <summary>
	/// ビューポートの設定
	/// </summary>
	/// <param name="kClientWidth"></param>
	/// <param name="kClientHeight"></param>
	/// <returns></returns>
	static D3D12_VIEWPORT viewportSetting(int32_t kClientWidth, int32_t kClientHeight);

	static D3D12_RECT scissorRectSetting(int32_t kClientWidth, int32_t kClientHeight);


	void EndFlame();

#pragma endregion

private:

	//でバック用
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

