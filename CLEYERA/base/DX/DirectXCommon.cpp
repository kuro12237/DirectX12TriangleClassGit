

#include"DirectXCommon.h"

DirectXCommon::DirectXCommon()
{
}

DirectXCommon::~DirectXCommon()
{
}

DirectXCommon* DirectXCommon::GetInstance()
{
	static DirectXCommon instance;
	return &instance;
}


void DirectXCommon::CreateDxgiFactory()
{

	IDXGIFactory7* dxgiFactory = DirectXCommon::GetInstance()->dxgiFactory_;
	IDXGIAdapter4* useAdapter = DirectXCommon::GetInstance()->useAdapter_;
	ID3D12Device* device = DirectXCommon::GetInstance()->device_;

#ifdef _DEBUG

	ID3D12Debug1* debugController = DirectXCommon::GetInstance()->debugController_;

	if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController))))
	{

		debugController->EnableDebugLayer();


		debugController->SetEnableGPUBasedValidation(TRUE);

	}
	DirectXCommon::GetInstance()->debugController_ = debugController;
#endif

	//DXGIファクトリーの生成
	dxgiFactory = nullptr;
	

	HRESULT hr = CreateDXGIFactory(IID_PPV_ARGS(&dxgiFactory));

	assert(SUCCEEDED(hr));

	//Adapter

	//いい順にアダプタを頼む
	for (UINT i = 0;dxgiFactory->EnumAdapterByGpuPreference(i,
		DXGI_GPU_PREFERENCE_HIGH_PERFORMANCE, IID_PPV_ARGS(&useAdapter)) !=
		DXGI_ERROR_NOT_FOUND; i++)
	{

		DXGI_ADAPTER_DESC3 adapterDesc{};
		hr = useAdapter->GetDesc3(&adapterDesc);
		assert(SUCCEEDED(hr));


		if (!(adapterDesc.Flags & DXGI_ADAPTER_FLAG3_SOFTWARE))
		{

			Log(ConvertString(std::format(L"Use Adapater:{}\n", adapterDesc.Description)));
			break;
		}


		useAdapter = nullptr;

	}

	assert(useAdapter != nullptr);


	DirectXCommon::GetInstance()->dxgiFactory_ = dxgiFactory;
	DirectXCommon::GetInstance()->useAdapter_ = useAdapter;
	DirectXCommon::GetInstance()->device_ = device;
}

void DirectXCommon::CreateDevice()
{
	D3D_FEATURE_LEVEL featureLevels[] =
	{
		D3D_FEATURE_LEVEL_12_2,D3D_FEATURE_LEVEL_12_1,D3D_FEATURE_LEVEL_12_0
	};
	const char* featureLevelStrings[] = { "12.2","12.1","12.0" };

	//高い順に生成できるか試していく
	for (size_t i = 0; i < _countof(featureLevels); i++)
	{

		HRESULT hr = D3D12CreateDevice(
			DirectXCommon::GetInstance()->useAdapter_,
			featureLevels[i],
			IID_PPV_ARGS(&DirectXCommon::GetInstance()->device_)
		);


		if (SUCCEEDED(hr))
		{

			Log(std::format("FeatureLevel : {}\n", featureLevelStrings[i]));
			break;

		}
	}

	assert(DirectXCommon::GetInstance()->device_ != nullptr);
}

void DirectXCommon::debugErrorInfoQueue()
{
	ID3D12InfoQueue* infoQueue = nullptr;
	ID3D12Device* device = DirectXCommon::GetInstance()->device_;

	if (SUCCEEDED(device->QueryInterface(IID_PPV_ARGS(&infoQueue))))
	{
		//やばいエラー時に止まる
		infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_CORRUPTION, true);

		//エラー時に止まる
		infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_ERROR, true);

		//警告時に止まる
		infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_WARNING, true);

		//
		//エラーと警告の抑制


		D3D12_MESSAGE_ID denyIds[] =
		{
			//windows11でのDXGIデバッグレイヤーとDX12デバッグレイヤーの相互バグによるエラーメッセージ
			//https:,,stackoverflow.com/questions/69805245/directx-12-application-is-crashing-in-windows-11

			D3D12_MESSAGE_ID_RESOURCE_BARRIER_MISMATCHING_COMMAND_LIST_TYPE
		};

		//抑制するレベル
		D3D12_MESSAGE_SEVERITY severities[] = { D3D12_MESSAGE_SEVERITY_INFO };
		D3D12_INFO_QUEUE_FILTER filter{};

		filter.DenyList.NumIDs = _countof(denyIds);
		filter.DenyList.pIDList = denyIds;
		filter.DenyList.NumSeverities = _countof(severities);
		filter.DenyList.pSeverityList = severities;

		//指定したメッセージの表示を抑制する
		infoQueue->PushStorageFilter(&filter);

		//解放
		infoQueue->Release();
	}
	DirectXCommon::GetInstance()->device_ = device;
}

void DirectXCommon::CreateCommands()
{
	ID3D12Device* device = DirectXCommon::GetInstance()->device_;
	Commands commands = DirectXCommon::GetInstance()->commands_;
	///commands
    //コマンドキューの生成
	commands.Queue = nullptr;
	HRESULT hr = device->CreateCommandQueue(&commands.QueueDesc, IID_PPV_ARGS(&commands.Queue));
	assert(SUCCEEDED(hr));

	//アローケーターの生成
    commands.Allocator = nullptr;
	hr = device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&commands.Allocator));
	assert(SUCCEEDED(hr));

	//リストの作成
	commands.List = nullptr;
	hr = device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT,
		commands.Allocator, nullptr,
		IID_PPV_ARGS(&commands.List));
	assert(SUCCEEDED(hr));

	DirectXCommon::GetInstance()->commands_ = commands;
	DirectXCommon::GetInstance()->device_ = device;
}

void DirectXCommon::CreateSwapChain()
{
	DirectXCommon::GetInstance()->swapChain.swapChain = nullptr;
	HWND hwnd_ = WinApp::GetInstance()->GetHwnd();

	DXGI_SWAP_CHAIN_DESC1 swapChainDesc{};

	//スワップチェーンの設定
	swapChainDesc.Width = WinApp::GetInstance()->GetkClinentWidth();
	swapChainDesc.Height = WinApp::GetInstance()->GetkClinentHeight();
	swapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.BufferCount = 2;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;

	
	//スワップチェーンの生成
	//HRESULT hr;
	DirectXCommon::GetInstance()->dxgiFactory_->CreateSwapChainForHwnd(DirectXCommon::GetInstance()->commands_.Queue, hwnd_, &swapChainDesc,
		nullptr, nullptr, reinterpret_cast<IDXGISwapChain1**>(&DirectXCommon::GetInstance()->swapChain.swapChain));
	//assert(SUCCEEDED(hr));

	DirectXCommon::GetInstance()->swapChainDesc = swapChainDesc;
	
}

ID3D12DescriptorHeap* DirectXCommon::CreateDescriptorHeap( D3D12_DESCRIPTOR_HEAP_TYPE heapType, UINT numDescriptors, bool shaderVisible)
{
	ID3D12DescriptorHeap* descriptHeap = nullptr;
	D3D12_DESCRIPTOR_HEAP_DESC descriptorHeapDesc{};
	descriptorHeapDesc.Type = heapType;
	descriptorHeapDesc.NumDescriptors = numDescriptors;
	descriptorHeapDesc.Flags = shaderVisible ? D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE : D3D12_DESCRIPTOR_HEAP_FLAG_NONE;

	//HRESULT hr = 
	DirectXCommon::GetInstance()->device_->CreateDescriptorHeap(&descriptorHeapDesc, IID_PPV_ARGS(&descriptHeap));
	//assert(SUCCEEDED(hr));
	return descriptHeap;

}

void DirectXCommon::CreatertvDescritorHeap()
{
	DirectXCommon::GetInstance()->rtv.DescritorHeap =
		CreateDescriptorHeap(D3D12_DESCRIPTOR_HEAP_TYPE_RTV, 2, false);
	DirectXCommon::GetInstance()->srvDescriptorHeap = 
		CreateDescriptorHeap(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, 128, true);
}

void DirectXCommon::CreateSwapChainResorce()
{
	HRESULT hr = DirectXCommon::GetInstance()->swapChain.swapChain->GetBuffer(0, IID_PPV_ARGS(&DirectXCommon::GetInstance()->swapChain.Resource[0]));


	assert(SUCCEEDED(hr));

	hr = DirectXCommon::GetInstance()->swapChain.swapChain->GetBuffer(1, IID_PPV_ARGS(&DirectXCommon::GetInstance()->swapChain.Resource[1]));

	assert(SUCCEEDED(hr));
}

void DirectXCommon::SettingandCreateRTV()
{
	RTV rtv = DirectXCommon::GetInstance()->rtv;
	ID3D12Device* device = DirectXCommon::GetInstance()->device_;

	SwapChain swapChain = DirectXCommon::GetInstance()->swapChain;


    rtv.rtvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
    rtv.rtvDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;

	rtv.rtvStartHandle = rtv.DescritorHeap->GetCPUDescriptorHandleForHeapStart();
	rtv.rtvHandles[0] = rtv.rtvStartHandle;
	device->CreateRenderTargetView(swapChain.Resource[0], &rtv.rtvDesc,
		rtv.rtvHandles[0]);


	rtv.rtvHandles[1].ptr = rtv.rtvHandles[0].ptr + device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

	device->CreateRenderTargetView(swapChain.Resource[1], &rtv.rtvDesc, rtv.rtvHandles[1]);

	DirectXCommon::GetInstance()->swapChain = swapChain;
	DirectXCommon::GetInstance()->rtv = rtv;
	DirectXCommon::GetInstance()->device_ = device;
}

void DirectXCommon::CreateFence()
{
	ID3D12Fence* fence = DirectXCommon::GetInstance()->fence;
	uint64_t fenceValue = DirectXCommon::GetInstance()->fenceValue;
	HANDLE fenceEvent = DirectXCommon::GetInstance()->fenceEvent;

	//フェンスの作成
	fence = nullptr;
	fenceValue = 0;
	//HRESULT hr =
	DirectXCommon::GetInstance()->device_->CreateFence(fenceValue, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&fence));
	//assert(SUCCEEDED(hr));


	fenceEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
	assert(fenceEvent != nullptr);

	DirectXCommon::GetInstance()->fence = fence;
	DirectXCommon::GetInstance()->fenceEvent = fenceEvent;
	DirectXCommon::GetInstance()->fenceValue = fenceValue;
}




void DirectXCommon::BeginFlame()
{
	SwapChain swapChain = DirectXCommon::GetInstance()->swapChain;
	Commands commands = DirectXCommon::GetInstance()->commands_;

	//書き込むスワップチェーンのindexをとる
	UINT backBufferIndex = swapChain.swapChain->GetCurrentBackBufferIndex();


	D3D12_RESOURCE_BARRIER barrier{};
	//バリア
    barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
    barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
    barrier.Transition.pResource = swapChain.Resource[backBufferIndex];
    barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT;
    barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;

	commands.List->ResourceBarrier(1, &barrier);
	DirectXCommon::GetInstance()->barrier = barrier;
	//描画先のRTYを設定
	commands.List->OMSetRenderTargets(1, &DirectXCommon::GetInstance()->rtv.rtvHandles[backBufferIndex], false, nullptr);

	float clearColor[] = { 0.1f,0.25f,0.5f,1.0f };
	//float clearColor[] = { 1.0f,0.0f,0.0f,1.0f };
	commands.List->ClearRenderTargetView(DirectXCommon::GetInstance()->rtv.rtvHandles[backBufferIndex], clearColor, 0, nullptr);

	DirectXCommon::GetInstance()->commands_ = commands;
	DirectXCommon::GetInstance()->swapChain = swapChain;

}

void DirectXCommon::EndFlame()
{
	Commands commands = DirectXCommon::GetInstance()->commands_;
	D3D12_RESOURCE_BARRIER barrier = DirectXCommon::GetInstance()->barrier;

	ID3D12Fence* fence = DirectXCommon::GetInstance()->fence;
	uint64_t fenceValue = DirectXCommon::GetInstance()->fenceValue;
	HANDLE fenceEvent = DirectXCommon::GetInstance()->fenceEvent;
	SwapChain swapChain = DirectXCommon::GetInstance()->swapChain;


	barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;
	barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT;

	commands.List->ResourceBarrier(1, &barrier);
	HRESULT hr = commands.List->Close();
	assert(SUCCEEDED(hr));


	ID3D12CommandList* commandLists[] = { DirectXCommon::GetInstance()->commands_.List };

	commands.Queue->ExecuteCommandLists(1, commandLists);
	swapChain.swapChain->Present(0, 1);


	//Fence数値加算
    fenceValue++;

	//GPUにシグナル
	commands.Queue->Signal(fence, fenceValue);


	if (fence->GetCompletedValue() < fenceValue)
	{

	    fence->SetEventOnCompletion(fenceValue, fenceEvent);


		WaitForSingleObject(fenceEvent, INFINITE);

	}

    swapChain.swapChain->Present(1, 0);


	hr = commands.Allocator->Reset();
	assert(SUCCEEDED(hr));

	hr =commands.List->Reset(commands.Allocator, nullptr);
	assert(SUCCEEDED(hr));

	DirectXCommon::GetInstance()->swapChain = swapChain;

	DirectXCommon::GetInstance()->barrier = barrier;
	DirectXCommon::GetInstance()->commands_ = commands;

	DirectXCommon::GetInstance()->fence = fence;
	DirectXCommon::GetInstance()->fenceEvent = fenceEvent;
	DirectXCommon::GetInstance()->fenceValue = fenceValue;
}

void DirectXCommon::ScissorViewCommand(const int32_t kClientWidth, const int32_t kClientHeight)
{
	D3D12_VIEWPORT viewport{};

	viewport = viewportSetting(kClientWidth, kClientHeight);

	//シザー矩形
	D3D12_RECT scissorRect{};
	scissorRect = scissorRectSetting(kClientWidth, kClientHeight);

	//コマンドを積む
	Commands commands = DirectXCommon::GetInstance()->commands_;

    commands.List->RSSetViewports(1, &viewport); //
    commands.List->RSSetScissorRects(1, &scissorRect);

	DirectXCommon::GetInstance()->commands_ = commands;
}


void DirectXCommon::FeanceReleace(ID3D12Fence* fence, HANDLE fenceEvent)
{
	CloseHandle(fenceEvent);
	fence->Release();
}

void DirectXCommon::DescripterRelease(RTV rtv, ID3D12DescriptorHeap* srvDescriptorHeap)
{

	rtv.DescritorHeap->Release();
	srvDescriptorHeap->Release();
}

void DirectXCommon::SwapChainRelease(SwapChain swapChain)
{
	swapChain.Resource[0]->Release();
	swapChain.Resource[1]->Release();
	swapChain.swapChain->Release();

}

void DirectXCommon::CommandsRelease(Commands commands)
{
	commands.List->Release();
	commands.Allocator->Release();
	commands.Queue->Release();
}

void DirectXCommon::Finalize()
{
	FeanceReleace(DirectXCommon::GetInstance()->fence,
		DirectXCommon::GetInstance()->fenceEvent);

	DescripterRelease(DirectXCommon::GetInstance()->rtv, 
		DirectXCommon::GetInstance()->srvDescriptorHeap);

	SwapChainRelease(DirectXCommon::GetInstance()->swapChain);
	CommandsRelease(DirectXCommon::GetInstance()->commands_);

	DirectXCommon::GetInstance()->device_->Release();
	DirectXCommon::GetInstance()->useAdapter_->Release();
	DirectXCommon::GetInstance()->dxgiFactory_->Release();
#ifdef _DEBUG

	DirectXCommon::GetInstance()->debugController_->Release();

#endif // _DEBUG

	
}

void DirectXCommon::ReleaseChack()
{
	IDXGIDebug1* debug;
	if (SUCCEEDED(DXGIGetDebugInterface1(0, IID_PPV_ARGS(&debug))))
	{
		debug->ReportLiveObjects(DXGI_DEBUG_ALL, DXGI_DEBUG_RLO_ALL);
		debug->ReportLiveObjects(DXGI_DEBUG_APP, DXGI_DEBUG_RLO_ALL);
		debug->ReportLiveObjects(DXGI_DEBUG_D3D12, DXGI_DEBUG_RLO_ALL);
		debug->Release();
	}


}

D3D12_VIEWPORT DirectXCommon::viewportSetting(int32_t kClientWidth, int32_t kClientHeight)
{

	D3D12_VIEWPORT viewport;

	//クライアント領域のサイズを一緒にして画面全体に表示
	viewport.Width = float(kClientWidth);
	viewport.Height = float(kClientHeight);
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;



	return viewport;
}

D3D12_RECT DirectXCommon::scissorRectSetting(int32_t kClientWidth, int32_t kClientHeight)
{
	//シザー矩形
	D3D12_RECT scissorRect{};

	//基本的にビューポートと同じ矩形が構成されるようにする
	scissorRect.left = 0;
	scissorRect.right = kClientWidth;
	scissorRect.top = 0;
	scissorRect.bottom = kClientHeight;


	return scissorRect;

}
