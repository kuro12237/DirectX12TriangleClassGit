#include "DirectXSetup.h"


#include"WinSetup.h"
#include<string>
#include<format>





std::wstring ConvertString(const std::string& str)
{
	if (str.empty())
	{
		return std::wstring();
	}

	auto sizeNeeded = MultiByteToWideChar(CP_UTF8, 0, reinterpret_cast<const char*>(&str[0]), static_cast<int>(str.size()), NULL, 0);
	if (sizeNeeded == 0)
	{
		return std::wstring();
	}
	std::wstring result(sizeNeeded, 0);
	MultiByteToWideChar(CP_UTF8, 0, reinterpret_cast<const char*>(&str[0]), static_cast<int>(str.size()), &result[0], sizeNeeded);
	return result;
}

std::string ConvertString(const std::wstring& str)
{
	if (str.empty())
	{
		return std::string();
	}

	auto sizeNeeded = WideCharToMultiByte(CP_UTF8, 0, str.data(), static_cast<int>(str.size()), NULL, 0, NULL, NULL);
	if (sizeNeeded == 0)
	{
		return std::string();
	}
	std::string result(sizeNeeded, 0);
	WideCharToMultiByte(CP_UTF8, 0, str.data(), static_cast<int>(str.size()), result.data(), sizeNeeded, NULL, NULL);
	return result;
}

//Log
void Log(const std::string& message)
{
	OutputDebugStringA(message.c_str());


}







/// <summary>
/// Compiler
/// </summary>
/// <param name="filePath"></param>
/// <param name="profile"></param>
/// <param name="dxcUtils"></param>
/// <param name="dxcCompiler"></param>
/// <param name="includeHandler"></param>
/// <returns></returns>
IDxcBlob* DirectXSetup::CompilerShader(
	//compilerするshaderファイルへのパス
	const std::wstring& filePath,
	//compilerに使用するProfile
	const wchar_t* profile,
	//初期化で生成したものを3つ
	IDxcUtils* dxcUtils,
	IDxcCompiler3* dxcCompiler,
	IDxcIncludeHandler* includeHandler)
{
	//1.hlslファイルを読む

	//これからシェーダーをコンパイルする旨をログに出す
	//Log(ConvertString(std::format(L"Begin CompileShader, path:{}, profile:{}\n", filePath, profile)));


	//1.hlslファイルを読む
	IDxcBlobEncoding* shaderSource = nullptr;
	HRESULT hr = dxcUtils->LoadFile(filePath.c_str(), nullptr, &shaderSource);

	//読めなかったら止める
	assert(SUCCEEDED(hr));

	//読み込んだファイルの内容の設定する
	DxcBuffer shaderSourceBuffer;
	shaderSourceBuffer.Ptr = shaderSource->GetBufferPointer();
	shaderSourceBuffer.Size = shaderSource->GetBufferSize();
	shaderSourceBuffer.Encoding = DXC_CP_UTF8; //UTF8の文字コードであることを通知


	//2.Compileする
	LPCWSTR arguments[] =
	{
		filePath.c_str(), //コンパイル対象のhlslファイル名
		L"-E",L"main", //エントリーポイントの指定。基本敵にmain以外にはしない
		L"-T",profile, //ShaderProfileの設定
		L"-Zi",L"-Qembed_debug", //デバッグ用の情報を埋め込む
		L"-Od", //最適化を外しておく
		L"-Zpr",//メモリレイアウトは行優先
	};

	//実際にShaderをコンパイルする
	IDxcResult* shaderResult = nullptr;
	hr = dxcCompiler->Compile(
		&shaderSourceBuffer, //読み込んだファイル
		arguments, //コンパイルオプション
		_countof(arguments), //コンパイルオプションの数
		includeHandler, //includeが含まれた諸々
		IID_PPV_ARGS(&shaderResult) //コンパイル結果
	);

	//コンパイルエラーではなくdxcが起動できないなど致命的な状況
	assert(SUCCEEDED(hr));



	//3.警告エラーがでていないか確認する

	//警告、エラーが出てたらログに出して止める
	IDxcBlobUtf8* shaderError = nullptr;
	shaderResult->GetOutput(DXC_OUT_ERRORS, IID_PPV_ARGS(&shaderError), nullptr);

	if (shaderError != nullptr && shaderError->GetStringLength() != 0)
	{
		Log(shaderError->GetStringPointer());

		//警告・エラーダメゼッタイ
		assert(false);
	}

	//4.Compile結果を受け取って返す

	//コンパイル結果から実行用のバイナリ部分を取得
	IDxcBlob* shaderBlob = nullptr;
	hr = shaderResult->GetOutput(DXC_OUT_OBJECT, IID_PPV_ARGS(&shaderBlob), nullptr);
	assert(SUCCEEDED(hr));

	//成功したログを出す
	Log(ConvertString(std::format(L"Compile Succeeded, path:{},profile:{}", filePath, profile)));

	//もう使わないリソースを解放
	shaderSource->Release();
	shaderResult->Release();

	//実行用のバイナリを返却
	return shaderBlob;

}


DirectXSetup::DirectXSetup()
{
	
}

DirectXSetup::~DirectXSetup()
{
	
}



void DirectXSetup::CreateDXGiFactory()
{


    #ifdef _DEBUG


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
	rtv.DescritorHeap= nullptr;

	rtv.rtvDescritorHeapDesc.Type= D3D12_DESCRIPTOR_HEAP_TYPE_RTV; //�����_�[�^�[�Q�b�g�r���[�p
	rtv.rtvDescritorHeapDesc.NumDescriptors = 2;//�_�u���o�b�t�@
	
	hr = device->CreateDescriptorHeap(&rtv.rtvDescritorHeapDesc, IID_PPV_ARGS(&rtv.DescritorHeap));
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

	rtv.rtvStartHandle= rtv.DescritorHeap->GetCPUDescriptorHandleForHeapStart();

	rtv.rtvHandles[0] = rtv.rtvStartHandle;
	device->CreateRenderTargetView(swapChain.Resource[0], &rtv.rtvDesc, rtv.rtvHandles[0]);

	//1�ڂ̃f�B�X�N���v�^�n���h���𓾂�(���͂�)
	rtv.rtvHandles[1].ptr = rtv.rtvHandles[0].ptr + device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

	//2�ڂ���
	device->CreateRenderTargetView(swapChain.Resource[1], &rtv.rtvDesc, rtv.rtvHandles[1]);


    //フェンスの作成
	fence = nullptr;
	fenceValue = 0;
	hr = device->CreateFence(fenceValue, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&fence));
	assert(SUCCEEDED(hr));

	//Fence��Signal��҂��߂̃C�x���g��쐬����
	fenceEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
	assert(fenceEvent != nullptr);
}

void DirectXSetup::DXCInitialize()
{
	dxc.Compiler = nullptr;
	dxc.Utils = nullptr;

	hr = DxcCreateInstance(CLSID_DxcUtils, IID_PPV_ARGS(&dxc.Utils));
	assert(SUCCEEDED(hr));

	hr = DxcCreateInstance(CLSID_DxcCompiler, IID_PPV_ARGS(&dxc.Compiler));
	assert(SUCCEEDED(hr));


}


void DirectXSetup::CreatePSO()
{

	IDxcIncludeHandler* includeHandler = nullptr;
	hr = dxc.Utils->CreateDefaultIncludeHandler(&includeHandler);
	assert(SUCCEEDED(hr));



	//RootSignature作成
	D3D12_ROOT_SIGNATURE_DESC descriptionRootSignature{};

	descriptionRootSignature.Flags =
		D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

	//シリアライズしてバイナリにする
	
	hr = D3D12SerializeRootSignature(&descriptionRootSignature,
		D3D_ROOT_SIGNATURE_VERSION_1, &signatureBlob, &errorBlob);
	if (FAILED(hr))
	{
		Log(reinterpret_cast<char*>(errorBlob->GetBufferPointer()));
		assert(false);
	}

	//バイナリを元に生成
	
	hr = device->CreateRootSignature(0, signatureBlob->GetBufferPointer(),
		signatureBlob->GetBufferSize(), IID_PPV_ARGS(&rootSignature));
	assert(SUCCEEDED(hr));




	//InputLayoutの設定
	D3D12_INPUT_ELEMENT_DESC inputElementDescs[1] = {};
	inputElementDescs[0].SemanticName = "POSITION";
	inputElementDescs[0].SemanticIndex = 0;
	inputElementDescs[0].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	inputElementDescs[0].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;
	D3D12_INPUT_LAYOUT_DESC inputLayoutDesc{};
	inputLayoutDesc.pInputElementDescs = inputElementDescs;
	inputLayoutDesc.NumElements = _countof(inputElementDescs);


	//BlendStateの設定を行う
	D3D12_BLEND_DESC blendDesc{};
	//すべての色要素を書き込む
	blendDesc.RenderTarget[0].RenderTargetWriteMask =
		D3D12_COLOR_WRITE_ENABLE_ALL;


	//RasterrizerStateぼ設定
	D3D12_RASTERIZER_DESC rasterizerDesc{};

	//裏面（時計回り）を表示しない
	rasterizerDesc.CullMode = D3D12_CULL_MODE_BACK;
	//三角形の中を塗りつぶす
	rasterizerDesc.FillMode = D3D12_FILL_MODE_SOLID;

	//Shaderをコンパイルする
	vertexShaderBlob = CompilerShader(L"Object3d.VS.hlsl",
		L"vs_6_0", dxc.Utils, dxc.Compiler, includeHandler);
	assert(vertexShaderBlob != nullptr);

	 pixeShaderBlob = CompilerShader(L"Object3d.PS.hlsl",
		L"ps_6_0", dxc.Utils, dxc.Compiler, includeHandler);
	assert(pixeShaderBlob != nullptr);

	//PSOの生成
	D3D12_GRAPHICS_PIPELINE_STATE_DESC graphicsPipelineStateDesc{};

	graphicsPipelineStateDesc.pRootSignature = rootSignature; //RootSignature
	graphicsPipelineStateDesc.InputLayout = inputLayoutDesc; //InputLayout
	graphicsPipelineStateDesc.VS = { vertexShaderBlob->GetBufferPointer(),
	vertexShaderBlob->GetBufferSize() }; //VertexShader
	graphicsPipelineStateDesc.PS = { pixeShaderBlob->GetBufferPointer(),
	pixeShaderBlob->GetBufferSize() }; //PixeShader
	graphicsPipelineStateDesc.BlendState = blendDesc; //BlendState
	graphicsPipelineStateDesc.RasterizerState = rasterizerDesc; //RasterizerState


	//書き込むRTVの情報
	graphicsPipelineStateDesc.NumRenderTargets = 1;
	graphicsPipelineStateDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;

	//利用するトポロジ(形状)のタイプ。三角形
	graphicsPipelineStateDesc.PrimitiveTopologyType =
		D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

	//どのように画面に色を打ち込むかの設定(気にしなくて良い)
	graphicsPipelineStateDesc.SampleDesc.Count = 1;
	graphicsPipelineStateDesc.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;

	hr = device->CreateGraphicsPipelineState(&graphicsPipelineStateDesc,
		IID_PPV_ARGS(&graphicsPipelineState));
	assert(SUCCEEDED(hr));

	vertexShaderBlob->Release();
	pixeShaderBlob->Release();

	
}




void DirectXSetup::BeginFlame(const int32_t kClientWidth, const int32_t kClientHeight)
{
	
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
	
	D3D12_VIEWPORT viewport{};

	//クライアント領域のサイズを一緒にして画面全体に表示
	viewport.Width = kClientWidth;
	viewport.Height = kClientHeight;
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;


	//シザー矩形
	D3D12_RECT scissorRect{};

	//基本的にビューポートと同じ矩形が構成されるようにする
	scissorRect.left = 0;
	scissorRect.right = kClientWidth;
	scissorRect.top = 0;
	scissorRect.bottom = kClientHeight;

	//コマンドを積む
	commands.List->RSSetViewports(1, &viewport); //
	commands.List->RSSetScissorRects(1, &scissorRect);
}

#pragma region 三角形
void DirectXSetup::CreateVecrtexResource()
{

	//頂点リソース用のヒープの設定
	D3D12_HEAP_PROPERTIES uploadHeapProperties{};
	uploadHeapProperties.Type = D3D12_HEAP_TYPE_UPLOAD; //UploadHeapを使う

	//頂点リソースの設定
	D3D12_RESOURCE_DESC vertexResourceDesc{};

	//バッファリソース。テクスチャの場合はまた別の設定をする
	vertexResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	vertexResourceDesc.Width = sizeof(Vec4) * 3; //リソースのサイズ。今回はvector4を3頂点分

	//バッファの場合はこれらは1にする決まり
	vertexResourceDesc.Height = 1;
	vertexResourceDesc.DepthOrArraySize = 1;
	vertexResourceDesc.MipLevels = 1;
	vertexResourceDesc.SampleDesc.Count = 1;

	//バッファの場合はこれにする決まり
	vertexResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	//実際に頂点リソースを作る
	vertexResource = nullptr;

	hr = device->CreateCommittedResource(&uploadHeapProperties, D3D12_HEAP_FLAG_NONE,
		&vertexResourceDesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&vertexResource));
	assert(SUCCEEDED(hr));

	vertexBufferView.BufferLocation = vertexResource->GetGPUVirtualAddress();

	//使用するリソースのサイズは頂点3つ分のサイズ
	vertexBufferView.SizeInBytes = sizeof(Vec4) * 3;

	//1頂点あたりのサイズ
	vertexBufferView.StrideInBytes = sizeof(Vec4);


}
void DirectXSetup::Draw(Vec4 top, Vec4 left, Vec4 right)
{

	Vec4* vertexData = nullptr;


	//書き込むためのアドレスを取得
	vertexResource->Map(0, nullptr, reinterpret_cast<void**>(&vertexData));

	//左下
	vertexData[0] = { left };

	//上
	vertexData[1] = { top };

	//右上
	vertexData[2] = { right};

	commands.List->SetGraphicsRootSignature(rootSignature);
	commands.List->SetPipelineState(graphicsPipelineState);//
	commands.List->IASetVertexBuffers(0, 1, &vertexBufferView);

	//形状を設定。PSOに設定しているものとはまた別。同じものを設定すると考えておけば良い
	commands.List->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//描画(DrawCall/ドローコール)。
	commands.List->DrawInstanced(3, 1, 0, 0);


}
#pragma endregion

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

    hr = commands.List->Reset(commands.Allocator, nullptr);
	assert(SUCCEEDED(hr));


	

}

void DirectXSetup::Deleate()
{


	CloseHandle(fenceEvent);
	fence->Release();

	rtv.DescritorHeap->Release();

	swapChain.Resource[0]->Release();
	swapChain.Resource[1]->Release();
	swapChain.swapChain->Release();

	commands.List->Release();
	commands.Allocator->Release();
	commands.Queue->Release();
	
	device->Release();
	useAdapter->Release();
	dxgiFactory->Release();

	vertexResource->Release();
	graphicsPipelineState->Release();

	signatureBlob->Release();
	if (errorBlob)
	{
		errorBlob->Release();
	}

	rootSignature->Release();


#ifdef _DEBUG

	debugController->Release();

#endif // _DEBUG

	
}
