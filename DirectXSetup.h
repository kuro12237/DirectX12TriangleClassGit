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

struct  Vec4
{
	float x, y, z, w;
};
struct  VertexProperty
{
	D3D12_VERTEX_BUFFER_VIEW vertex;
	ID3D12Resource* resouce;
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

struct SwapChain
{
	IDXGISwapChain4* swapChain;
	ID3D12Resource* Resource[2];


};

struct  RTV
{
	ID3D12DescriptorHeap* DescritorHeap;
	D3D12_DESCRIPTOR_HEAP_DESC rtvDescritorHeapDesc{};


	D3D12_RENDER_TARGET_VIEW_DESC rtvDesc{};
	D3D12_CPU_DESCRIPTOR_HANDLE rtvStartHandle;

	//RTVを2つ作るのでディスクリプタを2つ用意
	D3D12_CPU_DESCRIPTOR_HANDLE rtvHandles[2];
};

struct DXCProperty
{
	IDxcUtils*Utils;
	IDxcCompiler3* Compiler;

};

struct shader
{
	IDxcBlob* vertexBlob;
	IDxcBlob* pixeBlob;
};

//= rtvDescritorHeap->GetCPUDescriptorHandleForHeapStart();
class DirectXSetup
{
public:
	DirectXSetup();
	~DirectXSetup();

	static IDxcBlob* CompilerShader(
		const std::wstring& filePath,
		const wchar_t* profile,
		IDxcUtils* dxcUtils,
		IDxcCompiler3* dxcCompiler,
		IDxcIncludeHandler* includeHandler);

	/// <summary>
	/// DXGIファクトリーの生成
	/// </summary>
	void CreateDXGiFactory();

	/// <summary>
	/// デバイスの生成
	/// </summary>
	void CreateDevice();

	/// <summary>
	/// windows11でのDXGIデバッグレイヤーとDX12デバッグレイヤーの相互バグによるエラーチェック
	/// </summary>
	void DebugErrorInfoQueue();

	/// <summary>
	/// コマンドリストの生成
	/// </summary>
	/// 
	void CreateCommands();
	/// <summary>
	/// スワップチェーンの設定
	/// </summary>
	void CreateSwapChain(const int32_t Width, const int32_t Height,	HWND hwnd_);

	
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

	/// <summary>
	/// DXCの初期化
	/// </summary>
	void DXCInitialize();

	/// <summary>
	/// PSOの生成
	/// </summary>
	void CreatePSO();

	/// <summary>
	/// 三角形の頂点の生成
	/// </summary>
	void SetCreateVecrtexResource(VertexProperty &vertex);


	/// <summary>
	/// ループの一番最初に行う処理
	/// </summary>
	void BeginFlame(const int32_t kClientWidth, const int32_t kClientHeight);

	/// <summary>
	/// 三角形の描画
	/// </summary>
	void Draw(Vec4 top, Vec4 left, Vec4 right, VertexProperty vertex);

	/// <summary>
	/// ループの最後の処理
	/// </summary>
	void EndFlame();
	
	/// <summary>
	/// Release処理
	/// </summary>
	void Deleate();

	/// <summary>
	/// しっかりリリース処理されているかチェック
	/// </summary>
	void ChackRelease();

private:


	IDXGIFactory7* dxgiFactory = nullptr;
	IDXGIAdapter4* useAdapter = nullptr;
	ID3D12Device* device = nullptr;

	Commands commands;
	SwapChain swapChain;
	RTV rtv;

	DXCProperty dxc;


	//PSOの本体
	ID3D12PipelineState* graphicsPipelineState = nullptr;
	
	ID3D12RootSignature* rootSignature = nullptr;
	
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView{};

	HRESULT hr ;
	ID3D12Fence* fence = nullptr;
	uint64_t fenceValue;
	HANDLE fenceEvent;
	D3D12_RESOURCE_BARRIER barrier{};

	shader shader = {nullptr,nullptr};

	ID3DBlob* signatureBlob = nullptr;
	ID3DBlob* errorBlob = nullptr;

	IDxcBlob* vertexShaderBlob;
	IDxcBlob* pixeShaderBlob;

	ID3D12Debug1 *debugController = nullptr;

	//頂点リソース用のヒープの設定
	D3D12_HEAP_PROPERTIES uploadHeapProperties{};


};

