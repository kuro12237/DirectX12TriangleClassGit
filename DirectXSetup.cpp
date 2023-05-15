#include "DirectXSetup.h"

#include<d3d12.h>
#include<dxgi1_6.h>
#include<cassert>

#include <dxcapi.h>
#include<dxgidebug.h>

#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"dxgi.lib")

#pragma comment (lib,"dxguid.lib")
#pragma comment(lib,"dxcompiler.lib")

#include<Windows.h>
#include<string>
#include<format>

#include"Vector4.h"

DirectXSetup::DirectXSetup()
{

}

DirectXSetup::~DirectXSetup()
{
}




//Log
void LogDX(const std::string& message)
{
	OutputDebugStringA(message.c_str());


}

IDxcBlob* CompileShader(
	const std::wstring& filePath,
	const wchar_t* profile,
	IDxcUtils* dxcUtils,
	IDxcCompiler3* dxcCompiler,
	IDxcIncludeHandler* includeHandler)
{
	//_1
	//Log(ConvertString(std::format(L"Begin CompileShader,path:{},profile:{}\n", filePath, profile)));
	IDxcBlobEncoding* shaderSource = nullptr;
	HRESULT hr = dxcUtils->LoadFile(filePath.c_str(), nullptr, &shaderSource);
	assert(SUCCEEDED(hr));
	DxcBuffer shaderSourceBuffer;
	shaderSourceBuffer.Ptr = shaderSource->GetBufferPointer();
	shaderSourceBuffer.Size = shaderSource->GetBufferSize();
	shaderSourceBuffer.Encoding = DXC_CP_UTF8;
	//_2
	LPCWSTR arguments[] = {
		filePath.c_str(),
	L"-E",L"main",
	L"-T",profile,
	L"-Zi",L"-Qembed_debug",
	L"-Od",
	L"-Zpr",
	};

	IDxcResult* shaderResult = nullptr;

	hr = dxcCompiler->Compile(
		&shaderSourceBuffer,
		arguments,
		_countof(arguments),
		includeHandler,
		IID_PPV_ARGS(&shaderResult)
	);

	assert(SUCCEEDED(hr));

	//_3
	IDxcBlobUtf8* shaderError = nullptr;
	shaderResult->GetOutput(DXC_OUT_ERRORS, IID_PPV_ARGS(&shaderError), nullptr);
	if (shaderError != nullptr && shaderError->GetStringLength() != 0) {
		//Log(shaderError->GetStringPointer());
		assert(false);
	}
	//_4
	IDxcBlob* shaderBlob = nullptr;
	hr = shaderResult->GetOutput(DXC_OUT_OBJECT, IID_PPV_ARGS(&shaderBlob), nullptr);
	assert(SUCCEEDED(hr));
	//Log(ConvertString(std::format(L"Compile Shader,path:{},profile:{}\n", filePath, profile)));
	shaderSource->Release();
	shaderResult->Release();
	return  shaderBlob;
}


void DirectXSetup::DisplayInitalize(const int Width, const int Height, HWND hwnd)
{

	kCientHeight_ = Height;
	kCientWidth_ = Width;
#ifdef _DEBUG



	ID3D12Debug1* debugController = nullptr;
	if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController))))
	{
		debugController->EnableDebugLayer();
		debugController->SetEnableGPUBasedValidation(TRUE);
	}




#endif // _DEBUG

	//DXGIファクトリーの作成
	//DXGIファクトリーの作成
	IDXGIFactory7* dxgiFactory = nullptr;
	//HRESULTの作成


	hr = CreateDXGIFactory(IID_PPV_ARGS(&dxgiFactory));
	assert(SUCCEEDED(hr));
	//使用するアダプタの変数
	IDXGIAdapter4* useAdapter = nullptr;
	for (UINT i = 0; dxgiFactory->EnumAdapterByGpuPreference(i, DXGI_GPU_PREFERENCE_HIGH_PERFORMANCE,
		IID_PPV_ARGS(&useAdapter)) != DXGI_ERROR_NOT_FOUND; ++i)
	{
		DXGI_ADAPTER_DESC3 adapterDesc{};
		hr = useAdapter->GetDesc3(&adapterDesc);
		assert(SUCCEEDED(hr));
		//ソフトウェアアダプタ出なければ採用
		if (!(adapterDesc.Flags & DXGI_ADAPTER_FLAG3_SOFTWARE))
		{
			//LogDX(ConvertStringDX(std::format(L"Use Adapter:{}\n", adapterDesc.Description)));
			break;
		}
		useAdapter = nullptr;
	}



	//機能レベル
	D3D_FEATURE_LEVEL featureLevels[] =
	{
		D3D_FEATURE_LEVEL_12_2,D3D_FEATURE_LEVEL_12_1,D3D_FEATURE_LEVEL_12_0
	};
	const char* featureLevelStrings[] = { "12.2","12.1","12.0" };
	//高い順から生成
	for (size_t i = 0; i < _countof(featureLevels); i++)
	{
		hr = D3D12CreateDevice(useAdapter, featureLevels[i], IID_PPV_ARGS(&directX_.device));
		if (SUCCEEDED(hr))
		{
			//LogDX(std::format("FeatureLevel: {}\n", featureLevels[i]));
			break;
		}

	}
	assert(directX_.device != nullptr);
	LogDX("Compleate create D3D12Device!!\n");

#ifdef _DEBUG

	ID3D12InfoQueue* infoQueue = nullptr;
	if (SUCCEEDED(directX_.device->QueryInterface(IID_PPV_ARGS(&infoQueue))))
	{
		infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_CORRUPTION, true);
		infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_ERROR, true);
		D3D12_MESSAGE_ID denyIds[] =
		{


			//Windous11でのDXGIでバックレイヤーとDX12でバックレイヤーの総合作用によるエラーメッセージ
			//https://stackoverflow.com/questions/69805245/directx-12-application-is-crashing-in-windows-11
			D3D12_MESSAGE_ID_RESOURCE_BARRIER_MISMATCHING_COMMAND_LIST_TYPE
		};


		D3D12_MESSAGE_SEVERITY severities[] = { D3D12_MESSAGE_SEVERITY_INFO };
		D3D12_INFO_QUEUE_FILTER filter{};
		filter.DenyList.NumIDs = _countof(denyIds);
		filter.DenyList.pIDList = denyIds;
		filter.DenyList.NumSeverities = _countof(severities);
		filter.DenyList.pSeverityList = severities;
		infoQueue->PushStorageFilter(&filter);
		//解放
		infoQueue->Release();
	}

#endif // _DEBUG



	D3D12_COMMAND_QUEUE_DESC commandQueueDesc{};
	hr = directX_.device->CreateCommandQueue(&commandQueueDesc, IID_PPV_ARGS(&directX_.commandQueue));
	assert(SUCCEEDED(hr));



	hr = directX_.device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&directX_.commandAllocator));
	assert(SUCCEEDED(hr));


	hr = directX_.device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT,
		directX_.commandAllocator, nullptr, IID_PPV_ARGS(&directX_.commandList));
	assert(SUCCEEDED(hr));

	//スワップチェーンの生成


	directX_.swapChainDesc.Width = Width;
	directX_.swapChainDesc.Height = Height;
	directX_.swapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	directX_.swapChainDesc.SampleDesc.Count = 1;
	directX_.swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	directX_.swapChainDesc.BufferCount = 2;
	directX_.swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	hr = dxgiFactory->CreateSwapChainForHwnd(directX_.commandQueue, hwnd, &directX_.swapChainDesc,
		nullptr, nullptr, reinterpret_cast<IDXGISwapChain1**>(&directX_.swapChain));
	assert(SUCCEEDED(hr));


	//ヒープの作成
	ID3D12DescriptorHeap* rtvDescriptorheap = nullptr;
	D3D12_DESCRIPTOR_HEAP_DESC rtvDescDescriptorHeapDesc{};
	rtvDescDescriptorHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	rtvDescDescriptorHeapDesc.NumDescriptors = 2;
	hr = directX_.device->CreateDescriptorHeap(&rtvDescDescriptorHeapDesc, IID_PPV_ARGS(&rtvDescriptorheap));
	assert(SUCCEEDED(hr));

	hr = directX_.swapChain->GetBuffer(0, IID_PPV_ARGS(&directX_.swapChainResources[0]));
	//上手くいかない場合
	assert(SUCCEEDED(hr));
	hr = directX_.swapChain->GetBuffer(0, IID_PPV_ARGS(&directX_.swapChainResources[1]));
	//上手くいかない場合
	assert(SUCCEEDED(hr));

	//RTVの作成

	directX_.rtvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	directX_.rtvDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;//2Dテクスチャの書き込み
	directX_.rtvStartHandle = rtvDescriptorheap->GetCPUDescriptorHandleForHeapStart();
	directX_.rtvHandles[2];
	//一つ目
	directX_.rtvHandles[0] = directX_.rtvStartHandle;
	directX_.device->CreateRenderTargetView(directX_.swapChainResources[0], &directX_.rtvDesc, directX_.rtvHandles[0]);
	//二つ目
	directX_.rtvHandles[1].ptr = directX_.rtvHandles[0].ptr + directX_.device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
	directX_.device->CreateRenderTargetView(directX_.swapChainResources[1], &directX_.rtvDesc, directX_.rtvHandles[1]);







	directX_.backBufferIndex = directX_.swapChain->GetCurrentBackBufferIndex();

	//今回のバリアはTrannstion
	barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	//Noneにしておく
	barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
	barrier.Transition.pResource = directX_.swapChainResources[directX_.backBufferIndex];
	//遷移前のResou
	barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT;
	barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;

	directX_.commandList->ResourceBarrier(1, &barrier);

	directX_.commandList->OMSetRenderTargets(1, &directX_.rtvHandles[directX_.backBufferIndex], false, nullptr);

	float clearColor[] = { 0.1f,0.25f,0.5f,1.0f };

	directX_.commandList->ClearRenderTargetView(directX_.rtvHandles[directX_.backBufferIndex], clearColor, 0, nullptr);

	//DXCの初期化
	hr = DxcCreateInstance(CLSID_DxcUtils, IID_PPV_ARGS(&directX_.dxcUtils));
	assert(SUCCEEDED(hr));
	hr = DxcCreateInstance(CLSID_DxcCompiler, IID_PPV_ARGS(&directX_.dxcCompiler));
	assert(SUCCEEDED(hr));


	hr = directX_.device->CreateFence(directX_.fenceValue, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&directX_.fence));
	assert(SUCCEEDED(hr));



}

void DirectXSetup::SettingPSO()
{

	//includeするための対応設定
	hr = directX_.dxcUtils->CreateDefaultIncludeHandler(&directX_.includeHandler);
	assert(SUCCEEDED(hr));

	//PSOの作成
	directX_.descriptionRootSignature.Flags =
		D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

	ID3DBlob* signaltureBlob = nullptr;
	ID3D10Blob* errorBlob = nullptr;

	//シリアライズしてバイナリにする
	hr = D3D12SerializeRootSignature(&directX_.descriptionRootSignature,
		D3D_ROOT_SIGNATURE_VERSION_1, &signaltureBlob, &errorBlob);
	if (FAILED(hr))
	{
		LogDX(reinterpret_cast<char*>(errorBlob->GetBufferPointer()));
		assert(false);

	}


	hr = directX_.device->CreateRootSignature(0, signaltureBlob->GetBufferPointer(),
		signaltureBlob->GetBufferSize(), IID_PPV_ARGS(&directX_.rootsignature));
	assert(SUCCEEDED(hr));



	//InputLeyout
	directX_.inputElementDescs[0].SemanticName = "POSITION";
	directX_.inputElementDescs[0].SemanticIndex = 0;
	directX_.inputElementDescs[0].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	directX_.inputElementDescs[0].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;

	directX_.inputLayoutDesc.pInputElementDescs = directX_.inputElementDescs;
	directX_.inputLayoutDesc.NumElements = _countof(directX_.inputElementDescs);

	//Blendの設定
	directX_.blendDesc.RenderTarget[0].RenderTargetWriteMask =
		D3D12_COLOR_WRITE_ENABLE_ALL;

	//RasiterzerStateの設定
	directX_.rasterizerDesc.CullMode = D3D12_CULL_MODE_BACK;
	directX_.rasterizerDesc.FillMode = D3D12_FILL_MODE_SOLID;

	//shaderコンパイル
	directX_.vertexShaderBlob = CompileShader(L"Object3D.VS.hlsl", L"vs_6_0",
		directX_.dxcUtils, directX_.dxcCompiler, directX_.includeHandler);

	directX_.pixelShaderBlob = CompileShader(L"Object3D.PS.hlsl", L"ps_6_0",
		directX_.dxcUtils, directX_.dxcCompiler, directX_.includeHandler);


}

void DirectXSetup::CreatePSO()
{
	directX_.graphicsPipelineStateDesc.pRootSignature = directX_.rootsignature;
	directX_.graphicsPipelineStateDesc.InputLayout = directX_.inputLayoutDesc;
	directX_.graphicsPipelineStateDesc.VS = { directX_.vertexShaderBlob->GetBufferPointer(),directX_.vertexShaderBlob->GetBufferSize() };
	directX_.vertexShaderBlob->GetBufferSize();
	directX_.graphicsPipelineStateDesc.PS = { directX_.pixelShaderBlob->GetBufferPointer(),directX_.pixelShaderBlob->GetBufferSize() };
	directX_.pixelShaderBlob->GetBufferSize();
	directX_.graphicsPipelineStateDesc.BlendState = directX_.blendDesc;
	directX_.graphicsPipelineStateDesc.RasterizerState = directX_.rasterizerDesc;
	//書き込むRTVの情報
	directX_.graphicsPipelineStateDesc.NumRenderTargets = 1;
	directX_.graphicsPipelineStateDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	//利用するトポロジ(形状)のタイプ三角形
	directX_.graphicsPipelineStateDesc.PrimitiveTopologyType =
		D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

	//どのように画面に色を打ち込むのか設定
	directX_.graphicsPipelineStateDesc.SampleDesc.Count = 1;
	directX_.graphicsPipelineStateDesc.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;
	//実際に生成


	hr = directX_.device->CreateGraphicsPipelineState(&directX_.graphicsPipelineStateDesc,
		IID_PPV_ARGS(&directX_.graphicsPipelineState));
	assert(SUCCEEDED(hr));

}


void DirectXSetup::PolygonSetting(const int kClientWidth_, const int kClientHeight_)
{

}

void DirectXSetup::Draw(TrianglePos pos)
{



	//頂点リソース用のヒープの設定
	D3D12_HEAP_PROPERTIES uploadHEapProperties{};
	uploadHEapProperties.Type = D3D12_HEAP_TYPE_UPLOAD;
	//頂点リソースの設定
	D3D12_RESOURCE_DESC vertexResourceDesc{};
	vertexResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	vertexResourceDesc.Width = sizeof(Vec4) * 3;
	//バッファの場合はこれらを1にする
	vertexResourceDesc.Height = 1;
	vertexResourceDesc.DepthOrArraySize = 1;
	vertexResourceDesc.MipLevels = 1;
	vertexResourceDesc.SampleDesc.Count = 1;
	//バッファの場合
	vertexResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
	//頂点リソースの作成
	ID3D12Resource* vertexResource = nullptr;
	hr = directX_.device->CreateCommittedResource(&uploadHEapProperties, D3D12_HEAP_FLAG_NONE,
		&vertexResourceDesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&vertexResource));
	assert(SUCCEEDED(hr));


	//頂点バッファビューを作成する
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView{};
	vertexBufferView.BufferLocation = vertexResource->GetGPUVirtualAddress();
	//使用するリソースのサイズは頂点３つ文のサイズ
	vertexBufferView.SizeInBytes = sizeof(Vec4) * 3;
	//１頂点の当たりのサイズ
	vertexBufferView.StrideInBytes = sizeof(Vec4);


	D3D12_VIEWPORT viewport{};
	//クライアント領域のサイズと一緒にして画面全体に表示
	viewport.Width = 1280;// kClientWidth;
	viewport.Height = 720;//kClientHeight;
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;

	//シザー矩形 
	D3D12_RECT scissorRect{};
	//基本的にビューポートと同じ矩形が構成されるようにする
	scissorRect.left = 0;
	scissorRect.right = 1280;// kClientWidth;
	scissorRect.top = 0;
	scissorRect.bottom = 720;// kClientHeight;

	Vec4* vertexData = nullptr;
	vertexResource->Map(0, nullptr, reinterpret_cast<void**>(&vertexData));
	vertexData[0] = pos.left;
	vertexData[1] = pos.top;
	vertexData[2] = pos.right;





	////コマンドを積む
	directX_.commandList->RSSetViewports(1, &viewport);
	directX_.commandList->RSSetScissorRects(1, &scissorRect);
	//RootSignatureを設定。PSOに設定しているけど別途設定が必要
	directX_.commandList->SetGraphicsRootSignature(directX_.rootsignature);
	directX_.commandList->SetPipelineState(directX_.graphicsPipelineState);
	directX_.commandList->IASetVertexBuffers(0, 1, &vertexBufferView);
	//形状を設定。PSOに設定しているものとはまた別。同じものを設定すると考えよう
	directX_.commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	//描画のインスタンス
	directX_.commandList->DrawInstanced(3, 1, 0, 0);
	//今回はRendertargetからPresentにする

	/*


	barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;
	barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT;

	*/
}



void DirectXSetup::DisplayFinish()
{


}

void DirectXSetup::BeginFlame()
{

	hr = directX_.swapChain->GetBuffer(0, IID_PPV_ARGS(&directX_.swapChainResources[0]));
	//上手くいかない場合
	assert(SUCCEEDED(hr));
	hr = directX_.swapChain->GetBuffer(0, IID_PPV_ARGS(&directX_.swapChainResources[1]));
	//上手くいかない場合
	assert(SUCCEEDED(hr));

}

void DirectXSetup::EndFlame()
{
	//
//TransitionBarrierを張る

	barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;
	barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT;

	//今回のバリアはTrannstion
	barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;

	directX_.backBufferIndex = directX_.swapChain->GetCurrentBackBufferIndex();
	//Noneにしておく
	barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
	barrier.Transition.pResource = directX_.swapChainResources[directX_.backBufferIndex];

	directX_.commandList->ResourceBarrier(1, &barrier);

	// 画面に各処理はすべて終わり、画面に移すので状態遷移

	hr = directX_.commandList->Close();

	assert(SUCCEEDED(hr));

	//コマンドリストを実行
	ID3D12CommandList* commandLists[] = { directX_.commandList };
	directX_.commandQueue->ExecuteCommandLists(1, commandLists);


	directX_.fenceValue++;

	directX_.commandQueue->Signal(directX_.fence, directX_.fenceValue);
	directX_.swapChain->Present(1, 0);

	if (directX_.fence->GetCompletedValue() < directX_.fenceValue)
	{
		HANDLE fenceEvent = CreateEvent(NULL, false, false, NULL);
		assert(fenceEvent != nullptr);

		directX_.fence->SetEventOnCompletion(directX_.fenceValue, fenceEvent);
		WaitForSingleObject(fenceEvent, INFINITE);




	}


	hr = directX_.commandAllocator->Reset();
	assert(SUCCEEDED(hr));
	hr = directX_.commandList->Reset(directX_.commandAllocator, nullptr);
	assert(SUCCEEDED(hr));


}
