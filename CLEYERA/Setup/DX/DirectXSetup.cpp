

#include"DirectXSetup.h"

DirectXSetup::DirectXSetup()
{
}

DirectXSetup::~DirectXSetup()
{
}


void DirectXSetup::CreateDXGIFactorye()
{

#ifdef _DEBUG


	if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController))))
	{

		debugController->EnableDebugLayer();


		debugController->SetEnableGPUBasedValidation(TRUE);

	}

#endif

	//DXGI�t�@�N�g���[�̐���
	dxgiFactory = nullptr;
	

	hr = CreateDXGIFactory(IID_PPV_ARGS(&dxgiFactory));

	assert(SUCCEEDED(hr));

	//Adapter

	//�������ɃA�_�v�^�𗊂�
	for (UINT i = 0; dxgiFactory->EnumAdapterByGpuPreference(i,
		DXGI_GPU_PREFERENCE_HIGH_PERFORMANCE, IID_PPV_ARGS(&useAdapter)) !=
		DXGI_ERROR_NOT_FOUND; i++)
	{

		DXGI_ADAPTER_DESC3 adapterDesc{};
		hr = useAdapter->GetDesc3(&adapterDesc);
		assert(SUCCEEDED(hr));


		if (!(adapterDesc.Flags & DXGI_ADAPTER_FLAG3_SOFTWARE))
		{

			//Log(ConvertString(std::format(L"Use Adapater:{}\n", adapterDesc.Description)));
			break;
		}


		useAdapter = nullptr;

	}

	assert(useAdapter != nullptr);





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

		hr = D3D12CreateDevice(useAdapter, featureLevels[i], IID_PPV_ARGS(&device));


		if (SUCCEEDED(hr))
		{

			//Log(std::format("FeatureLevel : {}\n", featureLevelStrings[i]));
			break;

		}
	}

	assert(device != nullptr);




}

void DirectXSetup::debugErrorInfoQueue()
{
	ID3D12InfoQueue* infoQueue = nullptr;

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

}

void DirectXSetup::CreateCommands()
{

	///commands
    //�R�}���h�L���[�̐���
	commands.Queue = nullptr;
	hr = device->CreateCommandQueue(&commands.QueueDesc, IID_PPV_ARGS(&commands.Queue));
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
}

void DirectXSetup::CreateSwapChain(const int32_t Width, const int32_t Height, HWND hwnd_)
{
	swapChain.swapChain = nullptr;

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
	hr = dxgiFactory->CreateSwapChainForHwnd(commands.Queue, hwnd_, &swapChainDesc,
		nullptr, nullptr, reinterpret_cast<IDXGISwapChain1**>(&swapChain.swapChain));

	assert(SUCCEEDED(hr));
}

ID3D12DescriptorHeap* DirectXSetup::CreateDescriptorHeap( D3D12_DESCRIPTOR_HEAP_TYPE heapType, UINT numDescriptors, bool shaderVisible)
{
	ID3D12DescriptorHeap* descriptHeap = nullptr;
	D3D12_DESCRIPTOR_HEAP_DESC descriptorHeapDesc{};
	descriptorHeapDesc.Type = heapType;
	descriptorHeapDesc.NumDescriptors = numDescriptors;
	descriptorHeapDesc.Flags = shaderVisible ? D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE : D3D12_DESCRIPTOR_HEAP_FLAG_NONE;

	hr = device->CreateDescriptorHeap(&descriptorHeapDesc, IID_PPV_ARGS(&descriptHeap));
	assert(SUCCEEDED(hr));
	return descriptHeap;

}

void DirectXSetup::CreatertvDescritorHeap()
{
	
	rtv.DescritorHeap = CreateDescriptorHeap(D3D12_DESCRIPTOR_HEAP_TYPE_RTV, 2, false);
	srvDescriptorHeap = CreateDescriptorHeap(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, 128, true);



}

void DirectXSetup::CreateSwapChainResorce()
{
	hr = swapChain.swapChain->GetBuffer(0, IID_PPV_ARGS(&swapChain.Resource[0]));


	assert(SUCCEEDED(hr));

	hr = swapChain.swapChain->GetBuffer(1, IID_PPV_ARGS(&swapChain.Resource[1]));

	assert(SUCCEEDED(hr));
}

void DirectXSetup::SettingandCreateRTV()
{

	D3D12_RENDER_TARGET_VIEW_DESC rtvDesc{};
	rtvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	rtvDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;

	rtv.rtvStartHandle = rtv.DescritorHeap->GetCPUDescriptorHandleForHeapStart();

	rtv.rtvHandles[0] = rtv.rtvStartHandle;
	device->CreateRenderTargetView(swapChain.Resource[0], &rtvDesc, rtv.rtvHandles[0]);


	rtv.rtvHandles[1].ptr = rtv.rtvHandles[0].ptr + device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

	device->CreateRenderTargetView(swapChain.Resource[1], &rtvDesc, rtv.rtvHandles[1]);


}

void DirectXSetup::CreateFence()
{
	//�t�F���X�̍쐬
	fence = nullptr;
	fenceValue = 0;
	hr = device->CreateFence(fenceValue, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&fence));
	assert(SUCCEEDED(hr));


	fenceEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
	assert(fenceEvent != nullptr);


}

void DirectXSetup::BeginFlame()
{
	//�������ރX���b�v�`�F�[����index���Ƃ�
	UINT backBufferIndex = swapChain.swapChain->GetCurrentBackBufferIndex();

	//�o���A
	barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
	barrier.Transition.pResource = swapChain.Resource[backBufferIndex];
	barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT;
	barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;

	commands.List->ResourceBarrier(1, &barrier);

	//�`����RTY��ݒ�
	commands.List->OMSetRenderTargets(1, &rtv.rtvHandles[backBufferIndex], false, nullptr);

	float clearColor[] = { 0.1f,0.25f,0.5f,1.0f };
	//float clearColor[] = { 1.0f,0.0f,0.0f,1.0f };
	commands.List->ClearRenderTargetView(rtv.rtvHandles[backBufferIndex], clearColor, 0, nullptr);

}

void DirectXSetup::EndFlame()
{

	barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;
	barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT;

	commands.List->ResourceBarrier(1, &barrier);
	hr = commands.List->Close();
	assert(SUCCEEDED(hr));


	ID3D12CommandList* commandLists[] = { commands.List };

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

	hr = commands.List->Reset(commands.Allocator, nullptr);
	assert(SUCCEEDED(hr));

}

void DirectXSetup::ScissorViewCommand(const int32_t kClientWidth, const int32_t kClientHeight)
{
	D3D12_VIEWPORT viewport{};

	viewport = viewportSetting(kClientWidth, kClientHeight);

	//�V�U�[��`
	D3D12_RECT scissorRect{};
	scissorRect = scissorRectSetting(kClientWidth, kClientHeight);

	//�R�}���h��ς�
	commands.List->RSSetViewports(1, &viewport); //
	commands.List->RSSetScissorRects(1, &scissorRect);
	//commands.List->SetGraphicsRootSignature(rootSignature);
	//commands.List->SetPipelineState(graphicsPipelineState);//
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
