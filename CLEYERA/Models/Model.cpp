#include "Model.h"

Model::Model(){}

Model::~Model(){}

Model* Model::GetInstance()
{
	static Model instance;
	return &instance;
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

void Model::DefaultIncludeHandlerInitialize()
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


void Model::ShaderRelease()
{	
	FancShaderRelease(Model::GetInstance()->shader.shape);
}


void Model::Finalize()
{
	//図形
	PSORelese(Model::GetInstance()->Shape);	
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

void  Model::CreateResource()
{
	
	ID3D12Device* device_ = Model::GetInstance()->device;
	resource_.Vertex = CreateBufferResource(device_, sizeof(Vector4) * 3);
	resource_.Material = CreateBufferResource(device_, sizeof(Vector4));
	resource_.wvpResource = CreateBufferResource(device_, sizeof(Matrix4x4));
	resource_.BufferView = CreateBufferView(sizeof(Vector4) * 3,resource_.Vertex);
	


	Model::GetInstance()->device = device_;
	//return resultResource;

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


void Model::Initialize()
{
	pos_ = { 0,0,0 };
	size_ = 1;
	color_ = BLACK;
	CreateResource();
	worldTransform_.matWorld_= {
		1.0f,0.0f,0.0f,0.0f,
		0.0f,1.0f,0.0f,0.0f,
		0.0f,0.0f,1.0f,0.0f,
		0.0f,0.0f,0.0f,1.0f

	};


}

void Model::Draw()
{
	Vector4* vertexData = nullptr;
	Vector4* MaterialData = nullptr;
	Matrix4x4* wvpData = nullptr;
	//書き込むためのアドレスを取得
	resource_.Vertex->Map(0, nullptr, reinterpret_cast<void**>(&vertexData));
	resource_.Material->Map(0, nullptr, reinterpret_cast<void**>(&MaterialData));
	resource_.wvpResource->Map(0, nullptr, reinterpret_cast<void**>(&wvpData));

	

	Vector3 left =
	{ 
	  pos_.x - size_,
	  pos_.y - size_,
	  pos_.z
	};
	
	Vector3 right = 
	{
	   pos_.x + size_,
	   pos_.y - size_,
	   pos_.z
	};
	
	Vector3 top =
	{  
		pos_.x ,
		pos_.y + size_,
		pos_.z
	};

	//座標
	//左下
	vertexData[0] = { left.x,left.y,left.z , 1.0f};

	//上
	vertexData[1] = { top.x,top.y,top.z,1.0f};

	//右上
	vertexData[2] = { right.x,right.y,right.z,1.0f};

	//マテリアル
	Vector4 colorData = ColorCodeAdapter(color_);

	*MaterialData = colorData;

	//行列の変換
	
	*wvpData = worldTransform_.matWorld_;
	
	DrawCommand(Model::GetInstance()->commands,resource_, Model::GetInstance()->Shape);

}



void Model::DrawCommand(Commands commands, ResourcePeroperty Resource,PSOProperty PSO)
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

void Model::ResourceRelease()
{

	resource_.Vertex->Release();
	resource_.Material->Release();
	resource_.wvpResource->Release();
	
}

void Model::SetWorldTransform(WorldTransform worldTransform)
{
	worldTransform_ = worldTransform;
}

void Model::SetPos(Vector3 pos)
{
	pos_ = pos;
}

void Model::SetSize(float size)
{
	size_ = size;
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

