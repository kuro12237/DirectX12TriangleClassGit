#include"GraphicsPipeline.h"

GraphicsPipeline::GraphicsPipeline()
{
}

GraphicsPipeline::~GraphicsPipeline()
{
}

GraphicsPipeline* GraphicsPipeline::GetInstance()
{
	static GraphicsPipeline instance;
	return &instance;
}

void GraphicsPipeline::Initialize()
{
	//dxc�����
	GraphicsPipeline::GetInstance()->dxcCreate();
	//include�̑Ή��ݒ�
	GraphicsPipeline::GetInstance()->DfIncludeHandlerSetting();
}

void GraphicsPipeline::Finalize()
{
	//PSO�̉��
	PSORelease(GraphicsPipeline::GetInstance()->pso_.shape);

	//�V�F�[�_�[�̉��
	ShaderRelease(GraphicsPipeline::GetInstance()->shader_.shape);
	ShaderRelease(GraphicsPipeline::GetInstance()->shader_.sprite);

}

void GraphicsPipeline::ShaderCompile()
{
	ShaderMode shape, sprite;

	//
	shape.vertexBlob =
		GraphicsPipeline::CompilerShaderFanc(
			L"Shader/ShapeObject3d.VS.hlsl",
			L"vs_6_0"
		);
	shape.pixelBlob =
		GraphicsPipeline::CompilerShaderFanc(
			L"Shader/ShapeObject3d.PS.hlsl",
			L"ps_6_0"
		);
	//�X�v���C�g
	sprite.vertexBlob =
		GraphicsPipeline::CompilerShaderFanc(
			L"Shader/SpriteObject3d.VS.hlsl",
			L"vs_6_0"
		);
	sprite.pixelBlob =
		GraphicsPipeline::CompilerShaderFanc(
			L"Shader/SpriteObject3d.PS.hlsl",
			L"ps_6_0"
		);

	GraphicsPipeline::GetInstance()->shader_.shape = shape;
	GraphicsPipeline::GetInstance()->shader_.sprite = sprite;

}

void GraphicsPipeline::PSOCreate()
{

	ShapePSO();


}

void GraphicsPipeline::dxcCreate()
{

	HRESULT hr;

	hr = DxcCreateInstance(CLSID_DxcUtils, IID_PPV_ARGS(&GraphicsPipeline::GetInstance()->dxc.Utils));
	assert(SUCCEEDED(hr));

	hr = DxcCreateInstance(CLSID_DxcCompiler, IID_PPV_ARGS(&GraphicsPipeline::GetInstance()->dxc.Compiler));
	assert(SUCCEEDED(hr));
}

void GraphicsPipeline::DfIncludeHandlerSetting()
{
	HRESULT hr = 
		GraphicsPipeline::GetInstance()->dxc.Utils->CreateDefaultIncludeHandler(&GraphicsPipeline::GetInstance()->includeHandler);
	assert(SUCCEEDED(hr));


}

void GraphicsPipeline::ShapePSO()
{

	ID3D12Device* device = DirectXCommon::GetInstance()->GetDevice();
	Commands commands = DirectXCommon::GetInstance()->GetCommands();

	PSOProperty ShapePSO;
	
	ShaderMode shader = GraphicsPipeline::GetInstance()->shader_.shape;

	//RootSignature�쐬
	D3D12_ROOT_SIGNATURE_DESC descriptionRootSignature{};

	descriptionRootSignature.Flags =
		D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

	//Material�ݒ�
	D3D12_ROOT_PARAMETER rootParameters[2] = {};
	rootParameters[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	rootParameters[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
	rootParameters[0].Descriptor.ShaderRegister = 0;
	//Vertex��Transform
	rootParameters[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	rootParameters[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX;
	rootParameters[1].Descriptor.ShaderRegister = 0;


	descriptionRootSignature.pParameters = rootParameters;
	descriptionRootSignature.NumParameters = _countof(rootParameters);



	//�V���A���C�Y���ăo�C�i���ɂ���

	HRESULT hr = D3D12SerializeRootSignature(&descriptionRootSignature,
		D3D_ROOT_SIGNATURE_VERSION_1, &ShapePSO.signatureBlob, &ShapePSO.errorBlob);
	if (FAILED(hr))
	{
		Log(reinterpret_cast<char*>(ShapePSO.errorBlob->GetBufferPointer()));
		assert(false);
	}

	//�o�C�i�������ɐ���

	hr = device->CreateRootSignature(
		0,
		ShapePSO.signatureBlob->GetBufferPointer(),
		ShapePSO.signatureBlob->GetBufferSize(),
		IID_PPV_ARGS(&ShapePSO.rootSignature)
	);
	assert(SUCCEEDED(hr));




	//InputLayout�̐ݒ�
	D3D12_INPUT_ELEMENT_DESC inputElementDescs[1] = {};
	inputElementDescs[0].SemanticName = "POSITION";
	inputElementDescs[0].SemanticIndex = 0;
	inputElementDescs[0].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	inputElementDescs[0].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;
	D3D12_INPUT_LAYOUT_DESC inputLayoutDesc{};
	inputLayoutDesc.pInputElementDescs = inputElementDescs;
	inputLayoutDesc.NumElements = _countof(inputElementDescs);


	//BlendState�̐ݒ���s��
	D3D12_BLEND_DESC blendDesc{};
	//���ׂĂ̐F�v�f����������
	blendDesc.RenderTarget[0].RenderTargetWriteMask =
		D3D12_COLOR_WRITE_ENABLE_ALL;


	//RasterrizerState�ڐݒ�
	D3D12_RASTERIZER_DESC rasterizerDesc{};

	//���ʁi���v���j��\�����Ȃ�
	rasterizerDesc.CullMode = D3D12_CULL_MODE_BACK;
	//�O�p�`�̒���h��Ԃ�
	rasterizerDesc.FillMode = D3D12_FILL_MODE_SOLID;


	//PSO�̐���
	D3D12_GRAPHICS_PIPELINE_STATE_DESC graphicsPipelineStateDesc{};

	graphicsPipelineStateDesc.pRootSignature = ShapePSO.rootSignature; //RootSignature
	graphicsPipelineStateDesc.InputLayout = inputLayoutDesc; //InputLayout
	graphicsPipelineStateDesc.VS = { shader.vertexBlob->GetBufferPointer(),
	shader.vertexBlob->GetBufferSize() }; //VertexShader
	graphicsPipelineStateDesc.PS = { shader.pixelBlob->GetBufferPointer(),
	shader.pixelBlob->GetBufferSize() }; //PixeShader
	graphicsPipelineStateDesc.BlendState = blendDesc; //BlendState
	graphicsPipelineStateDesc.RasterizerState = rasterizerDesc; //RasterizerState


	//��������RTV�̏��
	graphicsPipelineStateDesc.NumRenderTargets = 1;
	graphicsPipelineStateDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;

	//���p����g�|���W(�`��)�̃^�C�v�B�O�p�`
	graphicsPipelineStateDesc.PrimitiveTopologyType =
		D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

	//�ǂ̂悤�ɉ�ʂɐF��ł����ނ��̐ݒ�(�C�ɂ��Ȃ��ėǂ�)
	graphicsPipelineStateDesc.SampleDesc.Count = 1;
	graphicsPipelineStateDesc.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;

	hr = device->CreateGraphicsPipelineState(&graphicsPipelineStateDesc,
		IID_PPV_ARGS(&ShapePSO.GraphicsPipelineState));
	assert(SUCCEEDED(hr));

	GraphicsPipeline::GetInstance()->pso_.shape = ShapePSO;


}

void GraphicsPipeline::ShaderRelease(ShaderMode shader)
{
	shader.pixelBlob->Release();
	shader.vertexBlob->Release();
}

void GraphicsPipeline::PSORelease(PSOProperty pso)
{
	pso.GraphicsPipelineState->Release();
	pso.signatureBlob->Release();
	if (pso.errorBlob)
	{
		pso.errorBlob->Release();
	}
	pso.rootSignature->Release();

}

////CompileShader�֐�
IDxcBlob* GraphicsPipeline::CompilerShaderFanc(
	const std::wstring& filePath,
	const wchar_t* profile
) {
	IDxcUtils* dxcUtils = GraphicsPipeline::GetInstance()->dxc.Utils;
	IDxcCompiler3* dxcCompiler = GraphicsPipeline::GetInstance()->dxc.Compiler;
	IDxcIncludeHandler* includeHandler = GraphicsPipeline::GetInstance()->includeHandler;

	//1.hlsl�t�@�C����ǂ�
	Log(ConvertString(std::format(L"Begin CompileShader,path:{},profile:{}\n", filePath, profile)));
	//hlsl�t�@�C����ǂ�
	IDxcBlobEncoding* shaderSource = nullptr;
	HRESULT hr = 
		dxcUtils->LoadFile(filePath.c_str(), nullptr, &shaderSource);
	//�ǂ߂Ȃ�������~�߂�
	assert(SUCCEEDED(hr));
	//�ǂݍ��񂾃t�@�C���̓��e��ݒ肷��
	DxcBuffer shaderSourceBuffer;
	shaderSourceBuffer.Ptr = shaderSource->GetBufferPointer();
	shaderSourceBuffer.Size = shaderSource->GetBufferSize();
	shaderSourceBuffer.Encoding = DXC_CP_UTF8;

	//2.Compile����
	LPCWSTR arguments[] = {
		filePath.c_str(),
		L"-E",L"main",
		L"-T",profile,
		L"-Zi",L"-Qembed_debug",
		L"-Od",
		L"-Zpr",
	};

	//���ۂ�Shader���R���p�C������
	IDxcResult* shaderResult = nullptr;
	hr = dxcCompiler->Compile(&shaderSourceBuffer, arguments, _countof(arguments), includeHandler, IID_PPV_ARGS(&shaderResult));
	//�R���p�C���G���[�ł͂Ȃ�dxc���N���o���Ȃ��Ȃǒv���I�ȏ�
	assert(SUCCEEDED(hr));


	//3.�x���E�G���[���o�Ă��Ȃ������m�F����
	//�x���E�G���[���o�Ă��烍�O�ɏo���Ď~�߂�
	IDxcBlobUtf8* shaderError = nullptr;
	shaderResult->GetOutput(DXC_OUT_ERRORS, IID_PPV_ARGS(&shaderError), nullptr);
	if (shaderError != nullptr && shaderError->GetStringLength() != 0) {
		Log(shaderError->GetStringPointer());
		assert(false);
	}


	//4.Compile���ʂ��󂯎���ĕԂ�
	//BLOB�E�E�EBinaryLargeOBject
	IDxcBlob* shaderBlob = nullptr;
	hr = shaderResult->GetOutput(DXC_OUT_OBJECT, IID_PPV_ARGS(&shaderBlob), nullptr);
	assert(SUCCEEDED(hr));
	//�����������O���o��
	Log(ConvertString(std::format(L"Compile Succeeded,path:{},profile:{}\n", filePath, profile)));
	//�����g��Ȃ����\�[�X�����
	shaderSource->Release();
	shaderResult->Release();
	//���s�p�̃o�C�i����ԋp
	return shaderBlob;


}