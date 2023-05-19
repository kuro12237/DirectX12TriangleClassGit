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
		//デバッグレイヤーを有効化する
		debugController->EnableDebugLayer();

		//さらにGRU側でもチェックを行うようにする	
		debugController->SetEnableGPUBasedValidation(TRUE);

	}
 
    #endif


	dxgiFactory = nullptr;
	hr=CreateDXGIFactory(IID_PPV_ARGS(&dxgiFactory));

	assert(SUCCEEDED(hr));
	//Adapter
	//いい順にアダプタを頼む
	for (UINT i = 0; dxgiFactory->EnumAdapterByGpuPreference(i,
		DXGI_GPU_PREFERENCE_HIGH_PERFORMANCE, IID_PPV_ARGS(&useAdapter)) !=
		DXGI_ERROR_NOT_FOUND; i++)
	{
		//アダプターの情報を取得する
		DXGI_ADAPTER_DESC3 adapterDesc{};
		hr = useAdapter->GetDesc3(&adapterDesc);
		assert(SUCCEEDED(hr)); //取得できないのは一大事

		//ソフトウェアアダプタでなければ採用！
		if (!(adapterDesc.Flags & DXGI_ADAPTER_FLAG3_SOFTWARE))
		{
			//採用したログを出力
			//Log(ConvertString(std::format(L"Use Adapater:{}\n", adapterDesc.Description)));
			break;
		}

		//ソフトウェアアダプタの場合は無視する
		useAdapter = nullptr;

	}
	//適切なアダプタがないので起動しない
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
	   //採用したアダプターでデバイスが生成できたかを確認
	   hr = D3D12CreateDevice(useAdapter, featureLevels[i], IID_PPV_ARGS(&device));

	   //指定した機能レベルでデバイスが生成できたかを確認
	   if (SUCCEEDED(hr))
	   {
		   //生成できたのでログ出力を行ってループを抜ける
		  // Log(std::format("FeatureLevel : {}\n", featureLevelStrings[i]));
		   break;

	   }
   }
   //デバイスの生成がうまくいかなかったので起動しない
   assert(device != nullptr);


}

void DirectXSetup::DebugErrorInfoQueue()
{
	ID3D12InfoQueue* infoQueue = nullptr;

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
}

void DirectXSetup::CreateCommands()
{
	///↓生成
	//コマンドキュー
	commands.Queue = nullptr;
	hr = device->CreateCommandQueue(&commands.QueueDesc, IID_PPV_ARGS(&commands.Queue));
	assert(SUCCEEDED(hr));

	//コマンドアローケータ
	commands.Allocator = nullptr;
	hr = device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&commands.Allocator));
    assert(SUCCEEDED(hr));
   
	//コマンドリスト
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
	swapChainDesc.SampleDesc.Count = 1; //マルチサンプルしない
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT; //描画のターゲットとして利用する
	swapChainDesc.BufferCount = 2; //ダブルバッファ
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD; //モニターにうつしたら、中身を破棄


	//コマンドキュー、ウィンドウハンドル、設定を渡して生成する
	hr = dxgiFactory->CreateSwapChainForHwnd(commands.Queue, hwnd_, &swapChainDesc,
		nullptr, nullptr, reinterpret_cast<IDXGISwapChain1**>(&swapChain.swapChain));

	assert(SUCCEEDED(hr));
}


void DirectXSetup::CreatertvDescritorHeap()
{
	rtv.rtvDescritorHeap= nullptr;

	rtv.rtvDescritorHeapDesc.Type= D3D12_DESCRIPTOR_HEAP_TYPE_RTV; //レンダーターゲットビュー用
	rtv.rtvDescritorHeapDesc.NumDescriptors = 2;//ダブルバッファ
	
	hr = device->CreateDescriptorHeap(&rtv.rtvDescritorHeapDesc, IID_PPV_ARGS(&rtv.rtvDescritorHeap));
	assert(SUCCEEDED(hr));
}

void DirectXSetup::CreateSwapChainResorce()
{
	hr = swapChain.swapChain->GetBuffer(0, IID_PPV_ARGS(&swapChain.Resource[0]));

	//うまく取得できなければ起動できない
	assert(SUCCEEDED(hr));

	hr = swapChain.swapChain->GetBuffer(1, IID_PPV_ARGS(&swapChain.Resource[1]));

	assert(SUCCEEDED(hr));

}

void DirectXSetup::SettingandCreateRTV()
{
	rtv.rtvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB; //出力結果をSRGBに変換して書き込む
	rtv.rtvDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D; //2dテクスチャとして書き込む

	rtv.rtvStartHandle= rtv.rtvDescritorHeap->GetCPUDescriptorHandleForHeapStart();

	rtv.rtvHandles[0] = rtv.rtvStartHandle;
	device->CreateRenderTargetView(swapChain.Resource[0], &rtv.rtvDesc, rtv.rtvHandles[0]);

	//1つ目のディスクリプタハンドルを得る(自力で)
	rtv.rtvHandles[1].ptr = rtv.rtvHandles[0].ptr + device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

	//2つ目を作る
	device->CreateRenderTargetView(swapChain.Resource[1], &rtv.rtvDesc, rtv.rtvHandles[1]);


	//初期値0でFenceを作る
	fence = nullptr;
	fenceValue = 0;
	hr = device->CreateFence(fenceValue, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&fence));
	assert(SUCCEEDED(hr));

	//FenceのSignalを待つためのイベントを作成する
	fenceEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
	assert(fenceEvent != nullptr);
}




void DirectXSetup::BeginFlame()
{
	fenceValue = 0;

	//バリア
	UINT backBufferIndex = swapChain.swapChain->GetCurrentBackBufferIndex();
    

	barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
	barrier.Transition.pResource = swapChain.Resource[backBufferIndex];
	barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT;
	barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;

	commands.List->ResourceBarrier(1, &barrier);

	//コマンドリスト
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


	//Fenceの値を更新
	fenceValue++;

	//GPUがここまでたどり着いたときに、Fenceの値を指定した値に代入するようにSignalを送る
	commands.Queue->Signal(fence, fenceValue);
	
	//Fenceの値が指定したSignal値にたどり着いているか確認する
	//GetCompletedValueの初期値はFence作成時に渡した初期値
	if (fence->GetCompletedValue() < fenceValue)
	{
		//指定したSignalにたどりついてないので、たどり着くまで待つようにイベントを設定する
		fence->SetEventOnCompletion(fenceValue, fenceEvent);

		//イベント待つ
		WaitForSingleObject(fenceEvent, INFINITE);

	}

	swapChain.swapChain->Present(1, 0);


	hr = commands.Allocator->Reset();
	assert(SUCCEEDED(hr));

    hr = commands.List->Reset(commands.Allocator, nullptr);
	assert(SUCCEEDED(hr));



}

void DirectXSetup::Deleate()
{
}
