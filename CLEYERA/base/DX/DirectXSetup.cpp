

#include"DirectXSetup.h"

DirectXSetup::DirectXSetup()
{
}

DirectXSetup::~DirectXSetup()
{
}

DirectXSetup* DirectXSetup::GetInstance()
{
	static DirectXSetup instance;
	return &instance;
}


void DirectXSetup::CreateDxgiFactory()
{

	IDXGIFactory7* dxgiFactory = DirectXSetup::GetInstance()->dxgiFactory_;
	IDXGIAdapter4* useAdapter = DirectXSetup::GetInstance()->useAdapter_;
	ID3D12Device* device = DirectXSetup::GetInstance()->device_;

#ifdef _DEBUG

	ID3D12Debug1* debugController = DirectXSetup::GetInstance()->debugController_;

	if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController))))
	{

		debugController->EnableDebugLayer();


		debugController->SetEnableGPUBasedValidation(TRUE);

	}
	DirectXSetup::GetInstance()->debugController_ = debugController;
#endif

	//DXGI�t�@�N�g���[�̐���
	dxgiFactory = nullptr;
	

	HRESULT hr = CreateDXGIFactory(IID_PPV_ARGS(&dxgiFactory));

	assert(SUCCEEDED(hr));

	//Adapter

	//�������ɃA�_�v�^�𗊂�
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


	DirectXSetup::GetInstance()->dxgiFactory_ = dxgiFactory;
	DirectXSetup::GetInstance()->useAdapter_ = useAdapter;
	DirectXSetup::GetInstance()->device_ = device;
}

void DirectXSetup::CreateDevice()
{
	D3D_FEATURE_LEVEL featureLevels[] =
	{
		D3D_FEATURE_LEVEL_12_2,D3D_FEATURE_LEVEL_12_1,D3D_FEATURE_LEVEL_12_0
	};
	const char* featureLevelStrings[] = { "12.2","12.1","12.0" };

	//�������ɐ����ł��邩�����Ă���
	for (size_t i = 0; i < _countof(featureLevels); i++)
	{

		HRESULT hr = D3D12CreateDevice(
			DirectXSetup::GetInstance()->useAdapter_,
			featureLevels[i],
			IID_PPV_ARGS(&DirectXSetup::GetInstance()->device_)
		);


		if (SUCCEEDED(hr))
		{

			Log(std::format("FeatureLevel : {}\n", featureLevelStrings[i]));
			break;

		}
	}

	assert(DirectXSetup::GetInstance()->device_ != nullptr);
}

void DirectXSetup::debugErrorInfoQueue()
{
	ID3D12InfoQueue* infoQueue = nullptr;
	ID3D12Device* device = DirectXSetup::GetInstance()->device_;

	if (SUCCEEDED(device->QueryInterface(IID_PPV_ARGS(&infoQueue))))
	{
		//��΂��G���[���Ɏ~�܂�
		infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_CORRUPTION, true);

		//�G���[���Ɏ~�܂�
		infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_ERROR, true);

		//�x�����Ɏ~�܂�
		infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_WARNING, true);

		//
		//�G���[�ƌx���̗}��


		D3D12_MESSAGE_ID denyIds[] =
		{
			//windows11�ł�DXGI�f�o�b�O���C���[��DX12�f�o�b�O���C���[�̑��݃o�O�ɂ��G���[���b�Z�[�W
			//https:,,stackoverflow.com/questions/69805245/directx-12-application-is-crashing-in-windows-11

			D3D12_MESSAGE_ID_RESOURCE_BARRIER_MISMATCHING_COMMAND_LIST_TYPE
		};

		//�}�����郌�x��
		D3D12_MESSAGE_SEVERITY severities[] = { D3D12_MESSAGE_SEVERITY_INFO };
		D3D12_INFO_QUEUE_FILTER filter{};

		filter.DenyList.NumIDs = _countof(denyIds);
		filter.DenyList.pIDList = denyIds;
		filter.DenyList.NumSeverities = _countof(severities);
		filter.DenyList.pSeverityList = severities;

		//�w�肵�����b�Z�[�W�̕\����}������
		infoQueue->PushStorageFilter(&filter);

		//���
		infoQueue->Release();
	}
	DirectXSetup::GetInstance()->device_ = device;
}

void DirectXSetup::CreateCommands()
{
	ID3D12Device* device = DirectXSetup::GetInstance()->device_;
	Commands commands = DirectXSetup::GetInstance()->commands;
	///commands
    //�R�}���h�L���[�̐���
	commands.Queue = nullptr;
	HRESULT hr = device->CreateCommandQueue(&commands.QueueDesc, IID_PPV_ARGS(&commands.Queue));
	assert(SUCCEEDED(hr));

	//�A���[�P�[�^�[�̐���
    commands.Allocator = nullptr;
	hr = device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&commands.Allocator));
	assert(SUCCEEDED(hr));

	//���X�g�̍쐬
	commands.List = nullptr;
	hr = device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT,
		commands.Allocator, nullptr,
		IID_PPV_ARGS(&commands.List));
	assert(SUCCEEDED(hr));

	DirectXSetup::GetInstance()->commands = commands;
	DirectXSetup::GetInstance()->device_ = device;
}

void DirectXSetup::CreateSwapChain(const int32_t Width, const int32_t Height, HWND hwnd_)
{
	DirectXSetup::GetInstance()->swapChain.swapChain = nullptr;

	DXGI_SWAP_CHAIN_DESC1 swapChainDesc{};

	//�X���b�v�`�F�[���̐ݒ�
	swapChainDesc.Width = Width;
	swapChainDesc.Height = Height;
	swapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.BufferCount = 2;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;

	
	//�X���b�v�`�F�[���̐���
	//HRESULT hr;
	DirectXSetup::GetInstance()->dxgiFactory_->CreateSwapChainForHwnd(DirectXSetup::GetInstance()->commands.Queue, hwnd_, &swapChainDesc,
		nullptr, nullptr, reinterpret_cast<IDXGISwapChain1**>(&DirectXSetup::GetInstance()->swapChain.swapChain));
	//assert(SUCCEEDED(hr));

	DirectXSetup::GetInstance()->swapChainDesc = swapChainDesc;
	
}

ID3D12DescriptorHeap* DirectXSetup::CreateDescriptorHeap( D3D12_DESCRIPTOR_HEAP_TYPE heapType, UINT numDescriptors, bool shaderVisible)
{
	ID3D12DescriptorHeap* descriptHeap = nullptr;
	D3D12_DESCRIPTOR_HEAP_DESC descriptorHeapDesc{};
	descriptorHeapDesc.Type = heapType;
	descriptorHeapDesc.NumDescriptors = numDescriptors;
	descriptorHeapDesc.Flags = shaderVisible ? D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE : D3D12_DESCRIPTOR_HEAP_FLAG_NONE;

	//HRESULT hr = 
	DirectXSetup::GetInstance()->device_->CreateDescriptorHeap(&descriptorHeapDesc, IID_PPV_ARGS(&descriptHeap));
	//assert(SUCCEEDED(hr));
	return descriptHeap;

}

void DirectXSetup::CreatertvDescritorHeap()
{
	DirectXSetup::GetInstance()->rtv.DescritorHeap =
		CreateDescriptorHeap(D3D12_DESCRIPTOR_HEAP_TYPE_RTV, 2, false);
	DirectXSetup::GetInstance()->srvDescriptorHeap = 
		CreateDescriptorHeap(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, 128, true);
}

void DirectXSetup::CreateSwapChainResorce()
{
	HRESULT hr = DirectXSetup::GetInstance()->swapChain.swapChain->GetBuffer(0, IID_PPV_ARGS(&DirectXSetup::GetInstance()->swapChain.Resource[0]));


	assert(SUCCEEDED(hr));

	hr = DirectXSetup::GetInstance()->swapChain.swapChain->GetBuffer(1, IID_PPV_ARGS(&DirectXSetup::GetInstance()->swapChain.Resource[1]));

	assert(SUCCEEDED(hr));
}

void DirectXSetup::SettingandCreateRTV()
{
	RTV rtv = DirectXSetup::GetInstance()->rtv;
	ID3D12Device* device = DirectXSetup::GetInstance()->device_;

	SwapChain swapChain = DirectXSetup::GetInstance()->swapChain;


    rtv.rtvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
    rtv.rtvDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;

	rtv.rtvStartHandle = rtv.DescritorHeap->GetCPUDescriptorHandleForHeapStart();
	rtv.rtvHandles[0] = rtv.rtvStartHandle;
	device->CreateRenderTargetView(swapChain.Resource[0], &rtv.rtvDesc,
		rtv.rtvHandles[0]);


	rtv.rtvHandles[1].ptr = rtv.rtvHandles[0].ptr + device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

	device->CreateRenderTargetView(swapChain.Resource[1], &rtv.rtvDesc, rtv.rtvHandles[1]);

	DirectXSetup::GetInstance()->swapChain = swapChain;
	DirectXSetup::GetInstance()->rtv = rtv;
	DirectXSetup::GetInstance()->device_ = device;
}

void DirectXSetup::CreateFence()
{
	ID3D12Fence* fence = DirectXSetup::GetInstance()->fence;
	uint64_t fenceValue = DirectXSetup::GetInstance()->fenceValue;
	HANDLE fenceEvent = DirectXSetup::GetInstance()->fenceEvent;

	//�t�F���X�̍쐬
	fence = nullptr;
	fenceValue = 0;
	//HRESULT hr =
	DirectXSetup::GetInstance()->device_->CreateFence(fenceValue, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&fence));
	//assert(SUCCEEDED(hr));


	fenceEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
	assert(fenceEvent != nullptr);

	DirectXSetup::GetInstance()->fence = fence;
	DirectXSetup::GetInstance()->fenceEvent = fenceEvent;
	DirectXSetup::GetInstance()->fenceValue = fenceValue;
}




void DirectXSetup::BeginFlame()
{
	SwapChain swapChain = DirectXSetup::GetInstance()->swapChain;
	Commands commands = DirectXSetup::GetInstance()->commands;

	//�������ރX���b�v�`�F�[����index���Ƃ�
	UINT backBufferIndex = swapChain.swapChain->GetCurrentBackBufferIndex();


	D3D12_RESOURCE_BARRIER barrier{};
	//�o���A
    barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
    barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
    barrier.Transition.pResource = swapChain.Resource[backBufferIndex];
    barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT;
    barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;

	commands.List->ResourceBarrier(1, &barrier);
	DirectXSetup::GetInstance()->barrier = barrier;
	//�`����RTY��ݒ�
	commands.List->OMSetRenderTargets(1, &DirectXSetup::GetInstance()->rtv.rtvHandles[backBufferIndex], false, nullptr);

	float clearColor[] = { 0.1f,0.25f,0.5f,1.0f };
	//float clearColor[] = { 1.0f,0.0f,0.0f,1.0f };
	commands.List->ClearRenderTargetView(DirectXSetup::GetInstance()->rtv.rtvHandles[backBufferIndex], clearColor, 0, nullptr);

	DirectXSetup::GetInstance()->commands = commands;
	DirectXSetup::GetInstance()->swapChain = swapChain;

}

void DirectXSetup::EndFlame()
{
	Commands commands = DirectXSetup::GetInstance()->commands;
	D3D12_RESOURCE_BARRIER barrier = DirectXSetup::GetInstance()->barrier;

	ID3D12Fence* fence = DirectXSetup::GetInstance()->fence;
	uint64_t fenceValue = DirectXSetup::GetInstance()->fenceValue;
	HANDLE fenceEvent = DirectXSetup::GetInstance()->fenceEvent;
	SwapChain swapChain = DirectXSetup::GetInstance()->swapChain;


	barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;
	barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT;

	commands.List->ResourceBarrier(1, &barrier);
	HRESULT hr = commands.List->Close();
	assert(SUCCEEDED(hr));


	ID3D12CommandList* commandLists[] = { DirectXSetup::GetInstance()->commands.List };

	commands.Queue->ExecuteCommandLists(1, commandLists);
	swapChain.swapChain->Present(0, 1);


	//Fence���l���Z
    fenceValue++;

	//GPU�ɃV�O�i��
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

	DirectXSetup::GetInstance()->swapChain = swapChain;

	DirectXSetup::GetInstance()->barrier = barrier;
	DirectXSetup::GetInstance()->commands = commands;

	DirectXSetup::GetInstance()->fence = fence;
	DirectXSetup::GetInstance()->fenceEvent = fenceEvent;
	DirectXSetup::GetInstance()->fenceValue = fenceValue;
}

void DirectXSetup::ScissorViewCommand(const int32_t kClientWidth, const int32_t kClientHeight)
{
	D3D12_VIEWPORT viewport{};

	viewport = viewportSetting(kClientWidth, kClientHeight);

	//�V�U�[��`
	D3D12_RECT scissorRect{};
	scissorRect = scissorRectSetting(kClientWidth, kClientHeight);

	//�R�}���h��ς�
	Commands commands = DirectXSetup::GetInstance()->commands;

    commands.List->RSSetViewports(1, &viewport); //
    commands.List->RSSetScissorRects(1, &scissorRect);

	DirectXSetup::GetInstance()->commands = commands;
}


void DirectXSetup::FeanceReleace(ID3D12Fence* fence, HANDLE fenceEvent)
{
	CloseHandle(fenceEvent);
	fence->Release();
}

void DirectXSetup::DescripterRelease(RTV rtv, ID3D12DescriptorHeap* srvDescriptorHeap)
{

	rtv.DescritorHeap->Release();
	srvDescriptorHeap->Release();
}

void DirectXSetup::SwapChainRelease(SwapChain swapChain)
{
	swapChain.Resource[0]->Release();
	swapChain.Resource[1]->Release();
	swapChain.swapChain->Release();

}

void DirectXSetup::CommandsRelease(Commands commands)
{
	commands.List->Release();
	commands.Allocator->Release();
	commands.Queue->Release();
}

void DirectXSetup::Finalize()
{
	FeanceReleace(DirectXSetup::GetInstance()->fence,
		DirectXSetup::GetInstance()->fenceEvent);

	DescripterRelease(DirectXSetup::GetInstance()->rtv, 
		DirectXSetup::GetInstance()->srvDescriptorHeap);

	SwapChainRelease(DirectXSetup::GetInstance()->swapChain);
	CommandsRelease(DirectXSetup::GetInstance()->commands);

	DirectXSetup::GetInstance()->device_->Release();
	DirectXSetup::GetInstance()->useAdapter_->Release();
	DirectXSetup::GetInstance()->dxgiFactory_->Release();
#ifdef _DEBUG

	DirectXSetup::GetInstance()->debugController_->Release();

#endif // _DEBUG

	
}

void DirectXSetup::ReleaseChack()
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

D3D12_VIEWPORT DirectXSetup::viewportSetting(int32_t kClientWidth, int32_t kClientHeight)
{

	D3D12_VIEWPORT viewport;

	//�N���C�A���g�̈�̃T�C�Y���ꏏ�ɂ��ĉ�ʑS�̂ɕ\��
	viewport.Width = float(kClientWidth);
	viewport.Height = float(kClientHeight);
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;



	return viewport;
}

D3D12_RECT DirectXSetup::scissorRectSetting(int32_t kClientWidth, int32_t kClientHeight)
{
	//�V�U�[��`
	D3D12_RECT scissorRect{};

	//��{�I�Ƀr���[�|�[�g�Ɠ�����`���\�������悤�ɂ���
	scissorRect.left = 0;
	scissorRect.right = kClientWidth;
	scissorRect.top = 0;
	scissorRect.bottom = kClientHeight;


	return scissorRect;

}
