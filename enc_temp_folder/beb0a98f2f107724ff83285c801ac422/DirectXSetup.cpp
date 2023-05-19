#include "DirectXSetup.h"

DirectXSetup::DirectXSetup()
{
	
}

DirectXSetup::~DirectXSetup()
{
}

void DirectXSetup::CreateDXGiFactory()
{


    #ifdef _DEBUG

	ID3D12Debug1* debugController = nullptr;
	if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController))))
	{
		//�f�o�b�O���C���[��L��������
		debugController->EnableDebugLayer();

		//�����GRU���ł�`�F�b�N��s���悤�ɂ���	
		debugController->SetEnableGPUBasedValidation(TRUE);

	}
 
    #endif


	dxgiFactory = nullptr;
	hr=CreateDXGIFactory(IID_PPV_ARGS(&dxgiFactory));

	assert(SUCCEEDED(hr));
	//Adapter
	//�������ɃA�_�v�^�𗊂�
	for (UINT i = 0; dxgiFactory->EnumAdapterByGpuPreference(i,
		DXGI_GPU_PREFERENCE_HIGH_PERFORMANCE, IID_PPV_ARGS(&useAdapter)) !=
		DXGI_ERROR_NOT_FOUND; i++)
	{
		//�A�_�v�^�[�̏���擾����
		DXGI_ADAPTER_DESC3 adapterDesc{};
		hr = useAdapter->GetDesc3(&adapterDesc);
		assert(SUCCEEDED(hr)); //�擾�ł��Ȃ��͈̂�厖

		//�\�t�g�E�F�A�A�_�v�^�łȂ���΍̗p�I
		if (!(adapterDesc.Flags & DXGI_ADAPTER_FLAG3_SOFTWARE))
		{
			//�̗p�������O��o��
			//Log(ConvertString(std::format(L"Use Adapater:{}\n", adapterDesc.Description)));
			break;
		}

		//�\�t�g�E�F�A�A�_�v�^�̏ꍇ�͖�������
		useAdapter = nullptr;

	}
	//�K�؂ȃA�_�v�^���Ȃ��̂ŋN�����Ȃ�
	assert(useAdapter != nullptr);

}




void DirectXSetup::CreateDevice()
{
   D3D_FEATURE_LEVEL featureLevels[]=
   {
	   D3D_FEATURE_LEVEL_12_2,D3D_FEATURE_LEVEL_12_1,D3D_FEATURE_LEVEL_12_0
   };
   const char* featureLevelStrings[] = { "12.2","12.1","12.0"};

   for (size_t i = 0; i < _countof(featureLevels); i++)
   {
	   //�̗p�����A�_�v�^�[�Ńf�o�C�X�������ł�������m�F
	   hr = D3D12CreateDevice(useAdapter, featureLevels[i], IID_PPV_ARGS(&device));

	   //�w�肵���@�\���x���Ńf�o�C�X�������ł�������m�F
	   if (SUCCEEDED(hr))
	   {
		   //�����ł����̂Ń��O�o�͂�s���ă��[�v�𔲂���
		  // Log(std::format("FeatureLevel : {}\n", featureLevelStrings[i]));
		   break;

	   }
   }
   //�f�o�C�X�̐��������܂������Ȃ������̂ŋN�����Ȃ�
   assert(device != nullptr);


}

void DirectXSetup::DebugErrorInfoQueue()
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
	///������
	//�R�}���h�L���[
	commands.Queue = nullptr;
	hr = device->CreateCommandQueue(&commands.QueueDesc, IID_PPV_ARGS(&commands.Queue));
	assert(SUCCEEDED(hr));

	//�R�}���h�A���[�P�[�^
	commands.Allocator = nullptr;
	hr = device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&commands.Allocator));
    assert(SUCCEEDED(hr));
   
	//�R�}���h���X�g
	commands.List = nullptr;
	hr=device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT,
		commands.Allocator, nullptr,
		IID_PPV_ARGS(&commands.List));
	assert(SUCCEEDED(hr));
}


void DirectXSetup::CreateSwapChain(const int32_t Width,const int32_t Height, HWND hwnd_)
{
    swapChain.swapChain = nullptr;
	DXGI_SWAP_CHAIN_DESC1 swapChainDesc{};

	swapChainDesc.Width = Width;
	swapChainDesc.Height = Height;
	swapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.SampleDesc.Count = 1; //�}���`�T���v�����Ȃ�
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT; //�`��̃^�[�Q�b�g�Ƃ��ė��p����
	swapChainDesc.BufferCount = 2; //�_�u���o�b�t�@
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD; //���j�^�[�ɂ�������A���g��j��


	//�R�}���h�L���[�A�E�B���h�E�n���h���A�ݒ��n���Đ�������
	hr = dxgiFactory->CreateSwapChainForHwnd(commands.Queue, hwnd_, &swapChainDesc,
		nullptr, nullptr, reinterpret_cast<IDXGISwapChain1**>(&swapChain.swapChain));

	assert(SUCCEEDED(hr));
}


void DirectXSetup::CreatertvDescritorHeap()
{
	rtv.rtvDescritorHeap= nullptr;

	rtv.rtvDescritorHeapDesc.Type= D3D12_DESCRIPTOR_HEAP_TYPE_RTV; //�����_�[�^�[�Q�b�g�r���[�p
	rtv.rtvDescritorHeapDesc.NumDescriptors = 2;//�_�u���o�b�t�@
	
	hr = device->CreateDescriptorHeap(&rtv.rtvDescritorHeapDesc, IID_PPV_ARGS(&rtv.rtvDescritorHeap));
	assert(SUCCEEDED(hr));
}

void DirectXSetup::CreateSwapChainResorce()
{
	hr = swapChain.swapChain->GetBuffer(0, IID_PPV_ARGS(&swapChain.Resource[0]));

	//���܂��擾�ł��Ȃ���΋N���ł��Ȃ�
	assert(SUCCEEDED(hr));

	hr = swapChain.swapChain->GetBuffer(1, IID_PPV_ARGS(&swapChain.Resource[1]));

	assert(SUCCEEDED(hr));

}

void DirectXSetup::SettingandCreateRTV()
{
	rtv.rtvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB; //�o�͌��ʂ�SRGB�ɕϊ����ď�������
	rtv.rtvDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D; //2d�e�N�X�`���Ƃ��ď�������

	rtv.rtvStartHandle= rtv.rtvDescritorHeap->GetCPUDescriptorHandleForHeapStart();

	rtv.rtvHandles[0] = rtv.rtvStartHandle;
	device->CreateRenderTargetView(swapChain.Resource[0], &rtv.rtvDesc, rtv.rtvHandles[0]);

	//1�ڂ̃f�B�X�N���v�^�n���h���𓾂�(���͂�)
	rtv.rtvHandles[1].ptr = rtv.rtvHandles[0].ptr + device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

	//2�ڂ���
	device->CreateRenderTargetView(swapChain.Resource[1], &rtv.rtvDesc, rtv.rtvHandles[1]);


	//�����l0��Fence����
	fence = nullptr;
	fenceValue = 0;
	hr = device->CreateFence(fenceValue, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&fence));
	assert(SUCCEEDED(hr));

	//Fence��Signal��҂��߂̃C�x���g��쐬����
	fenceEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
	assert(fenceEvent != nullptr);
}




void DirectXSetup::BeginFlame()
{
	fenceValue = 0;

	//�o���A
	UINT backBufferIndex = swapChain.swapChain->GetCurrentBackBufferIndex();
    

	barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
	barrier.Transition.pResource = swapChain.Resource[backBufferIndex];
	barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT;
	barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;

	commands.List->ResourceBarrier(1, &barrier);

	//�R�}���h���X�g
	commands.List->OMSetRenderTargets(1, &rtv.rtvHandles[backBufferIndex], false, nullptr);
	
	float clearColor[] = { 0.1f,0.25f,0.5f,1.0f };
	
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


	//Fence�̒l��X�V
	fenceValue++;

	//GPU�������܂ł��ǂ蒅�����Ƃ��ɁAFence�̒l��w�肵���l�ɑ������悤��Signal�𑗂�
	commands.Queue->Signal(fence, fenceValue);
	
	//Fence�̒l���w�肵��Signal�l�ɂ��ǂ蒅���Ă��邩�m�F����
	//GetCompletedValue�̏����l��Fence�쐬���ɓn���������l
	if (fence->GetCompletedValue() < fenceValue)
	{
		//�w�肵��Signal�ɂ��ǂ���ĂȂ��̂ŁA���ǂ蒅���܂ő҂悤�ɃC�x���g��ݒ肷��
		fence->SetEventOnCompletion(fenceValue, fenceEvent);

		//�C�x���g�҂�
		WaitForSingleObject(fenceEvent, INFINITE);

	}

	swapChain.swapChain->Present(1, 0);


	hr = commands.Allocator->Reset();
	assert(SUCCEEDED(hr));

    hr = commands.List->Reset(commands.Allocator, nullptr);
	assert(SUCCEEDED(hr));


	hr = directX_.commandAllocator->Reset();
	assert(SUCCEEDED(hr));
	hr = directX_.commandList->Reset(directX_.commandAllocator, nullptr);
	assert(SUCCEEDED(hr));

}

void DirectXSetup::Deleate()
{
}
