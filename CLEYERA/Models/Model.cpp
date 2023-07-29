#include "Model.h"

Model::Model()
{
}

Model::~Model()
{
}



Model* Model::GetInstance()
{
	static Model instance;
	return &instance;
}
void Model::Initialize()
{
}

void Model::SetDevice(ID3D12Device*device_)
{
	Model::GetInstance()->device = device_;

}

void Model::SetCommands(Commands command_)
{
	Model::GetInstance()->commands = command_;

}

void Model::dxcInitialize()
{
	DXCProperty ResultDxc = { nullptr,nullptr };

	HRESULT hr;
	
	hr = DxcCreateInstance(CLSID_DxcUtils, IID_PPV_ARGS(&Model::GetInstance()->dxc.Utils));
	assert(SUCCEEDED(hr));

	hr = DxcCreateInstance(CLSID_DxcCompiler, IID_PPV_ARGS(&Model::GetInstance()->dxc.Compiler));
	assert(SUCCEEDED(hr));

}

void Model::InitializeDfIncludeHandler()
{


	HRESULT hr = Model::GetInstance()->dxc.Utils->CreateDefaultIncludeHandler(&Model::GetInstance()->includeHandler);
	assert(SUCCEEDED(hr));



}



////CompileShader関数
IDxcBlob* Model::CompilerShader(
	const std::wstring& filePath,
	const wchar_t* profile,
	IDxcUtils* dxcUtils,
	IDxcCompiler3* dxcCompiler,
	IDxcIncludeHandler* includeHandler) {
	//1.hlslファイルを読む
	Log(ConvertString(std::format(L"Begin CompileShader,path:{},profile:{}\n", filePath, profile)));
	//hlslファイルを読む
	IDxcBlobEncoding* shaderSource = nullptr;
	HRESULT hr = dxcUtils->LoadFile(filePath.c_str(), nullptr, &shaderSource);
	//読めなかったら止める
	assert(SUCCEEDED(hr));
	//読み込んだファイルの内容を設定する
	DxcBuffer shaderSourceBuffer;
	shaderSourceBuffer.Ptr = shaderSource->GetBufferPointer();
	shaderSourceBuffer.Size = shaderSource->GetBufferSize();
	shaderSourceBuffer.Encoding = DXC_CP_UTF8;

	//2.Compileする
	LPCWSTR arguments[] = {
		filePath.c_str(),
		L"-E",L"main",
		L"-T",profile,
		L"-Zi",L"-Qembed_debug",
		L"-Od",
		L"-Zpr",
	};

	//実際にShaderをコンパイルする
	IDxcResult* shaderResult = nullptr;
	hr = dxcCompiler->Compile(&shaderSourceBuffer, arguments, _countof(arguments), includeHandler, IID_PPV_ARGS(&shaderResult));
	//コンパイルエラーではなくdxcが起動出来ないなど致命的な状況
	assert(SUCCEEDED(hr));


	//3.警告・エラーが出ていないかを確認する
	//警告・エラーが出てたらログに出して止める
	IDxcBlobUtf8* shaderError = nullptr;
	shaderResult->GetOutput(DXC_OUT_ERRORS, IID_PPV_ARGS(&shaderError), nullptr);
	if (shaderError != nullptr && shaderError->GetStringLength() != 0) {
		Log(shaderError->GetStringPointer());
		assert(false);
	}


	//4.Compile結果を受け取って返す
	//BLOB・・・BinaryLargeOBject
	IDxcBlob* shaderBlob = nullptr;
	hr = shaderResult->GetOutput(DXC_OUT_OBJECT, IID_PPV_ARGS(&shaderBlob), nullptr);
	assert(SUCCEEDED(hr));
	//成功したログを出す
	Log(ConvertString(std::format(L"Compile Succeeded,path:{},profile:{}\n", filePath, profile)));
	//もう使わないリソースを解放
	shaderSource->Release();
	shaderResult->Release();
	//実行用のバイナリを返却
	return shaderBlob;


}

void Model::CompileShaders()
{
	Shaders shader;

	DXCProperty dxc = Model::GetInstance()->dxc;
	IDxcIncludeHandler* includeHandler = Model::GetInstance()->includeHandler;

	//Shaderをコンパイルする
	shader.shape.vertexBlob = CompilerShader(L"Shader/ShapeObject3d.VS.hlsl",
		L"vs_6_0", dxc.Utils, dxc.Compiler, includeHandler);
	assert(shader.shape.vertexBlob != nullptr);

	shader.shape.pixelBlob = CompilerShader(L"Shader/ShapeObject3d.PS.hlsl",
		L"ps_6_0", dxc.Utils, dxc.Compiler, includeHandler);
	assert(shader.shape.pixelBlob != nullptr);

	shader.sprite.vertexBlob = CompilerShader(L"shader/SpriteObject3d.VS.hlsl",
		L"vs_6_0", dxc.Utils, dxc.Compiler, includeHandler);
	assert(shader.sprite.vertexBlob);

	shader.sprite.pixelBlob = CompilerShader(L"shader/SpriteObject3d.PS.hlsl",
		L"ps_6_0", dxc.Utils, dxc.Compiler, includeHandler);
	assert(shader.sprite.pixelBlob);

	Model::GetInstance()->shader = shader;

}

void Model::ShapeCreatePSO()
{

	PSOProperty Shape;
	ID3D12Device* device = Model::GetInstance()->device;
	Commands commands = Model::GetInstance()->commands;
	Shaders shader = Model::GetInstance()->shader;

	HRESULT hr;

	//RootSignature作成
	D3D12_ROOT_SIGNATURE_DESC descriptionRootSignature{};

	descriptionRootSignature.Flags =
		D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

	//Material設定
	D3D12_ROOT_PARAMETER rootParameters[2] = {};
	rootParameters[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	rootParameters[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
	rootParameters[0].Descriptor.ShaderRegister = 0;
	//VertexのTransform
	rootParameters[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	rootParameters[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX;
	rootParameters[1].Descriptor.ShaderRegister = 0;


	descriptionRootSignature.pParameters = rootParameters;
	descriptionRootSignature.NumParameters = _countof(rootParameters);



	//シリアライズしてバイナリにする

	hr = D3D12SerializeRootSignature(&descriptionRootSignature,
		D3D_ROOT_SIGNATURE_VERSION_1, &Shape.signatureBlob, &Shape.errorBlob);
	if (FAILED(hr))
	{
		Log(reinterpret_cast<char*>(Shape.errorBlob->GetBufferPointer()));
		assert(false);
	}

	//バイナリを元に生成

	hr = device->CreateRootSignature(0, Shape.signatureBlob->GetBufferPointer(),
		Shape.signatureBlob->GetBufferSize(), IID_PPV_ARGS(&Shape.rootSignature));
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


	//PSOの生成
	D3D12_GRAPHICS_PIPELINE_STATE_DESC graphicsPipelineStateDesc{};

	graphicsPipelineStateDesc.pRootSignature = Shape.rootSignature; //RootSignature
	graphicsPipelineStateDesc.InputLayout = inputLayoutDesc; //InputLayout
	graphicsPipelineStateDesc.VS = { shader.shape.vertexBlob->GetBufferPointer(),
	shader.shape.vertexBlob->GetBufferSize() }; //VertexShader
	graphicsPipelineStateDesc.PS = { shader.shape.pixelBlob->GetBufferPointer(),
	shader.shape.pixelBlob->GetBufferSize() }; //PixeShader
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
		IID_PPV_ARGS(&Shape.GraphicsPipelineState));
	assert(SUCCEEDED(hr));

	Model::GetInstance()->Shape= Shape;
    Model::GetInstance()->device=device;
	Model::GetInstance()->commands = commands;
	Model::GetInstance()->shader = shader;


}


void Model::SpriteCreatePSO()
{
	PSOProperty Sprite;
	ID3D12Device* device = Model::GetInstance()->device;
	Commands commands = Model::GetInstance()->commands;
	Shaders shader = Model::GetInstance()->shader;


	//RootSignature作成
	D3D12_ROOT_SIGNATURE_DESC descriptionRootSignature{};

	//descriptionRootSignature = CreateDescriptRootSignature();
	descriptionRootSignature.Flags =
		D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

	//Material設定
	D3D12_ROOT_PARAMETER rootParameters[3] = {};
	rootParameters[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	rootParameters[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
	rootParameters[0].Descriptor.ShaderRegister = 0;

	//VertexのTransform
	rootParameters[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	rootParameters[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX;
	rootParameters[1].Descriptor.ShaderRegister = 0;


	//DescriptorRanged
	D3D12_DESCRIPTOR_RANGE descriptorRange[1] = {};
	descriptorRange[0].BaseShaderRegister = 0;
	descriptorRange[0].NumDescriptors = 1;
	descriptorRange[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	descriptorRange[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	//rootPrameterに入れる
	rootParameters[2].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	rootParameters[2].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
	rootParameters[2].DescriptorTable.pDescriptorRanges = descriptorRange;
	rootParameters[2].DescriptorTable.NumDescriptorRanges = _countof(descriptorRange);


	//Samplerの設定
	D3D12_STATIC_SAMPLER_DESC staticSamplers[1] = {};
	staticSamplers[0].Filter = D3D12_FILTER_COMPARISON_MIN_MAG_MIP_LINEAR;
	staticSamplers[0].AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	staticSamplers[0].AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	staticSamplers[0].AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;

	staticSamplers[0].ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
	staticSamplers[0].MaxLOD = D3D12_FLOAT32_MAX;
	staticSamplers[0].ShaderRegister = 0;
	staticSamplers[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;

	descriptionRootSignature.pStaticSamplers = staticSamplers;
	descriptionRootSignature.NumStaticSamplers = _countof(staticSamplers);




	descriptionRootSignature.pParameters = rootParameters;
	descriptionRootSignature.NumParameters = _countof(rootParameters);

	//シリアライズしてバイナリにする

	HRESULT hr = D3D12SerializeRootSignature(&descriptionRootSignature,
		D3D_ROOT_SIGNATURE_VERSION_1, &Sprite.signatureBlob, &Sprite.errorBlob);
	if (FAILED(hr))
	{
		Log(reinterpret_cast<char*>(Sprite.errorBlob->GetBufferPointer()));
		assert(false);
	}

	//バイナリを元に生成

	hr = device->CreateRootSignature(0, Sprite.signatureBlob->GetBufferPointer(),
		Sprite.signatureBlob->GetBufferSize(), IID_PPV_ARGS(&Sprite.rootSignature));
	assert(SUCCEEDED(hr));




	//InputLayoutの設定
	D3D12_INPUT_ELEMENT_DESC inputElementDescs[2] = {};
	inputElementDescs[0].SemanticName = "POSITION";
	inputElementDescs[0].SemanticIndex = 0;
	inputElementDescs[0].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	inputElementDescs[0].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;

	inputElementDescs[1].SemanticName = "TEXCOORD";
	inputElementDescs[1].SemanticIndex = 0;
	inputElementDescs[1].Format = DXGI_FORMAT_R32G32_FLOAT;
	inputElementDescs[1].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;




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


	//PSOの生成
	D3D12_GRAPHICS_PIPELINE_STATE_DESC graphicsPipelineStateDesc{};

	graphicsPipelineStateDesc.pRootSignature = Sprite.rootSignature; //RootSignature
	graphicsPipelineStateDesc.InputLayout = inputLayoutDesc; //InputLayout
	graphicsPipelineStateDesc.VS = { shader.sprite.vertexBlob->GetBufferPointer(),
	shader.sprite.vertexBlob->GetBufferSize() }; //VertexShader
	graphicsPipelineStateDesc.PS = { shader.sprite.pixelBlob->GetBufferPointer(),
	shader.sprite.pixelBlob->GetBufferSize() }; //PixeShader
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
		IID_PPV_ARGS(&Sprite.GraphicsPipelineState));
	assert(SUCCEEDED(hr));

	Model::GetInstance()->Sprite = Sprite;
	Model::GetInstance()->device = device;
	Model::GetInstance()->commands = commands;
	Model::GetInstance()->shader = shader;
}




void Model::ShaderRelease()
{

	
	FancShaderRelease(Model::GetInstance()->shader.shape);
	FancShaderRelease(Model::GetInstance()->shader.sprite);
}


void Model::Finalize()
{
	//図形
	PSORelese(Model::GetInstance()->Shape);

	//sprite
	PSORelese(Model::GetInstance()->Sprite);
	
}

ID3D12Resource* Model::CreateBufferResource(ID3D12Device* device, size_t sizeInbyte)
{
	ID3D12Resource* RssultResource;
	//頂点リソース用のヒープの設定
	D3D12_HEAP_PROPERTIES uploadHeapProperties{};

	uploadHeapProperties.Type = D3D12_HEAP_TYPE_UPLOAD; //UploadHeapを使う

	//頂点リソースの設定
	D3D12_RESOURCE_DESC ResourceDesc{};


	//バッファリソース。テクスチャの場合はまた別の設定をする
	ResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	ResourceDesc.Width = sizeInbyte; //リソースのサイズ。今回はvector4を3頂点分

	//バッファの場合はこれらは1にする決まり
	ResourceDesc.Height = 1;
	ResourceDesc.DepthOrArraySize = 1;
	ResourceDesc.MipLevels = 1;
	ResourceDesc.SampleDesc.Count = 1;

	//バッファの場合はこれにする決まり
	ResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
	HRESULT hr;
	hr = device->CreateCommittedResource(&uploadHeapProperties, D3D12_HEAP_FLAG_NONE,
		&ResourceDesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&RssultResource));
	assert(SUCCEEDED(hr));

	return RssultResource;
}

D3D12_VERTEX_BUFFER_VIEW Model::CreateBufferView(size_t sizeInbyte, ID3D12Resource* Resource)
{
	D3D12_VERTEX_BUFFER_VIEW resultBufferView;

	resultBufferView.BufferLocation = Resource->GetGPUVirtualAddress();

	//使用するリソースのサイズは頂点3つ分のサイズ
	resultBufferView.SizeInBytes = UINT(sizeInbyte);

	//1頂点あたりのサイズ
	resultBufferView.StrideInBytes = UINT(sizeInbyte / 3);
	return resultBufferView;

}

ResourcePeroperty  Model::CreateShapeResource()
{
	ResourcePeroperty resultResource;
	ID3D12Device* device = Model::GetInstance()->device;
	resultResource.Vertex = CreateBufferResource(device, sizeof(Vector4) * 3);
	resultResource.Material = CreateBufferResource(device, sizeof(Vector4));
	resultResource.wvpResource = CreateBufferResource(device, sizeof(Matrix4x4));
	resultResource.BufferView = CreateBufferView(sizeof(Vector4) * 3,resultResource.Vertex);
	Model::GetInstance()->device = device;
	return resultResource;

}

Vector4 Model::ColorCodeAdapter(unsigned int color)
{
	Vector4 ResultColor = {

	   ((color >> 24) & 0xFF) / 255.0f, // 赤

	   ((color >> 16) & 0xFF) / 255.0f, // 緑

	   ((color >> 8) & 0xFF) / 255.0f,  // 青

	   ((color) & 0xFF) / 255.0f //透明度

	};

	return ResultColor;

}

ResourcePeroperty Model::CreateTriangleSpriteResource()
{
	ResourcePeroperty resultResource;
	ID3D12Device* device = Model::GetInstance()->device;
	resultResource.Vertex = CreateBufferResource(device, sizeof(VertexData) * 3);
	resultResource.Material = CreateBufferResource(device, sizeof(Vector4));
	resultResource.wvpResource = CreateBufferResource(device, sizeof(Matrix4x4));
	resultResource.BufferView = CreateBufferView(sizeof(VertexData) * 3, resultResource.Vertex);
	Model::GetInstance()->device = device;

	return resultResource;


}

void Model::ShapeDraw(Position position, Vector4 color, Matrix4x4 worldTransform,ResourcePeroperty Resource)
{
	Vector4* vertexData = nullptr;
	Vector4* MaterialData = nullptr;
	Matrix4x4* wvpData = nullptr;
	//書き込むためのアドレスを取得
	Resource.Vertex->Map(0, nullptr, reinterpret_cast<void**>(&vertexData));
	Resource.Material->Map(0, nullptr, reinterpret_cast<void**>(&MaterialData));
	Resource.wvpResource->Map(0, nullptr, reinterpret_cast<void**>(&wvpData));

	//座標
	//左下
	vertexData[0] = { position.left.x,position.left.y,position.left.z , 1.0f};

	//上
	vertexData[1] = { position.top.x,position.top.y,position.top.z,1.0f};

	//右上
	vertexData[2] = { position.right.x,position.right.y,position.right.z,1.0f};

	//マテリアル
	//Vector4 colorData = ColorCodeAdapter(ColorCode);

	*MaterialData = color;

	//行列の変換
	
	*wvpData = worldTransform;
	
	ShapeDrawCommands(Model::GetInstance()->commands,Resource, Model::GetInstance()->Shape);

}



void Model::ShapeDrawCommands(Commands commands, ResourcePeroperty Resource,PSOProperty PSO)
{


	commands.List->SetGraphicsRootSignature(PSO.rootSignature);
	commands.List->SetPipelineState(PSO.GraphicsPipelineState);//

	commands.List->IASetVertexBuffers(0, 1, &Resource.BufferView);

	//形状を設定。PSOに設定しているものとはまた別。同じものを設定すると考えておけば良い
	commands.List->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//マテリアルCBufferの場所を設定
	commands.List->SetGraphicsRootConstantBufferView(0, Resource.Material->GetGPUVirtualAddress());


	//wvp用のCBufferの場所を設定
	commands.List->SetGraphicsRootConstantBufferView(1, Resource.wvpResource->GetGPUVirtualAddress());

	//描画(DrawCall/ドローコール)。
	commands.List->DrawInstanced(3, 1, 0, 0);


}

void Model::ShapeResourceRelease(ResourcePeroperty Resource)
{

	Resource.Vertex->Release();
	Resource.Material->Release();
	Resource.wvpResource->Release();
	
}



void Model::TriangleSpriteDraw(Position position, unsigned int color, Matrix4x4 worldTransform, ResourcePeroperty Resource, texResourceProperty tex)
{

	VertexData* vertexData = nullptr;
	//Vector4* vertexData = nullptr;
	Vector4* MaterialData = nullptr;
	Matrix4x4* wvpData = nullptr;
	//書き込むためのアドレスを取得
	Resource.Vertex->Map(0, nullptr, reinterpret_cast<void**>(&vertexData));
	Resource.Material->Map(0, nullptr, reinterpret_cast<void**>(&MaterialData));
	Resource.wvpResource->Map(0, nullptr, reinterpret_cast<void**>(&wvpData));

	vertexData[0].position = { position.left.x,position.left.y,position.left.z,1.0f };

	vertexData[0].texcoord = { 0.0f,1.0f };
	////上
	vertexData[1].position = {position.top.x, position.top.y, position.top.z, 1.0f};
	vertexData[1].texcoord = { 0.5f,0.0f };
	////右
	vertexData[2].position = { position.right.x,position.right.y,position.right.z,1.0f };
	vertexData[2].texcoord = { 1.0f,1.0f };

	//マテリアル
	Vector4 colorData = ColorCodeAdapter(color);

	*MaterialData = colorData;

	//行列の変換

	*wvpData = worldTransform;

	SpriteDrawCommands(Resource, tex,Model::GetInstance()->commands, Model::GetInstance()->Sprite);

}

void Model::SpriteDrawCommands(ResourcePeroperty Resource, texResourceProperty tex,Commands commands, PSOProperty PSO)
{

	commands.List->SetGraphicsRootSignature(PSO.rootSignature);
	commands.List->SetPipelineState(PSO.GraphicsPipelineState);//

	commands.List->IASetVertexBuffers(0, 1, &Resource.BufferView);

	//形状を設定。PSOに設定しているものとはまた別。同じものを設定すると考えておけば良い
	commands.List->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//マテリアルCBufferの場所を設定
	commands.List->SetGraphicsRootConstantBufferView(0, Resource.Material->GetGPUVirtualAddress());


	//wvp用のCBufferの場所を設定
	commands.List->SetGraphicsRootConstantBufferView(1, Resource.wvpResource->GetGPUVirtualAddress());

	//
	commands.List->SetGraphicsRootDescriptorTable(2, tex.SrvHandleGPU);


	//描画(DrawCall/ドローコール)。
	commands.List->DrawInstanced(3, 1, 0, 0);

}

void Model::PSORelese(PSOProperty PSO)
{
	PSO.GraphicsPipelineState->Release();
    PSO.signatureBlob->Release();
	if (PSO.errorBlob)
	{
		PSO.errorBlob->Release();
	}
	PSO.rootSignature->Release();


}

void Model::FancShaderRelease(Mode shader)
{
	shader.pixelBlob->Release();
	shader.vertexBlob->Release();

}

void Model::TriangleSpriteResourceRelease(ResourcePeroperty &Resource, texResourceProperty &tex)
{

	Resource.Vertex->Release();
	Resource.Material->Release();
	Resource.wvpResource->Release();

    tex.Resource->Release();
	
}
