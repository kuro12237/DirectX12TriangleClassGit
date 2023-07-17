#include "Model.h"

Model::Model()
{
}

void Model::Initialize()
{
}

void Model::SetDevice(ID3D12Device*device_)
{
	device = device_;

}

void Model::SetCommands(Commands command_)
{

	commands = command_;
}

void Model::dxcInitialize()
{
	DXCProperty ResultDxc = { nullptr,nullptr };

	HRESULT hr;
	
	hr = DxcCreateInstance(CLSID_DxcUtils, IID_PPV_ARGS(&dxc.Utils));
	assert(SUCCEEDED(hr));

	hr = DxcCreateInstance(CLSID_DxcCompiler, IID_PPV_ARGS(&dxc.Compiler));
	assert(SUCCEEDED(hr));

}

void Model::InitializeDfIncludeHandler()
{


	HRESULT hr = dxc.Utils->CreateDefaultIncludeHandler(&includeHandler);
	assert(SUCCEEDED(hr));



}



////CompileShader�֐�
IDxcBlob* Model::CompilerShader(
	const std::wstring& filePath,
	const wchar_t* profile,
	IDxcUtils* dxcUtils,
	IDxcCompiler3* dxcCompiler,
	IDxcIncludeHandler* includeHandler) {
	//1.hlsl�t�@�C����ǂ�
	//Log(ConvertString(std::format(L"Begin CompileShader,path:{},profile:{}\n", filePath, profile)));
	//hlsl�t�@�C����ǂ�
	IDxcBlobEncoding* shaderSource = nullptr;
	HRESULT hr = dxcUtils->LoadFile(filePath.c_str(), nullptr, &shaderSource);
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
		//Log(shaderError->GetStringPointer());
		assert(false);
	}


	//4.Compile���ʂ��󂯎���ĕԂ�
	//BLOB�E�E�EBinaryLargeOBject
	IDxcBlob* shaderBlob = nullptr;
	hr = shaderResult->GetOutput(DXC_OUT_OBJECT, IID_PPV_ARGS(&shaderBlob), nullptr);
	assert(SUCCEEDED(hr));
	//�����������O���o��
	//Log(ConvertString(std::format(L"Compile Succeeded,path:{},profile:{}\n", filePath, profile)));
	//�����g��Ȃ����\�[�X�����
	shaderSource->Release();
	shaderResult->Release();
	//���s�p�̃o�C�i����ԋp
	return shaderBlob;


}

void Model::CompileShaders()
{
	//Shader���R���p�C������
	shader.vertexBlob = CompilerShader(L"Shader/Object3d.VS.hlsl",
		L"vs_6_0", dxc.Utils, dxc.Compiler, includeHandler);
	assert(shader.vertexBlob != nullptr);

	shader.pixelBlob = CompilerShader(L"Shader/Object3d.PS.hlsl",
		L"ps_6_0", dxc.Utils, dxc.Compiler, includeHandler);
	assert(shader.pixelBlob != nullptr);
}

void Model::ShapeCreatePSO()
{
	HRESULT hr;

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

	hr = D3D12SerializeRootSignature(&descriptionRootSignature,
		D3D_ROOT_SIGNATURE_VERSION_1, &Shape.signatureBlob, &Shape.errorBlob);
	if (FAILED(hr))
	{
		//Log(reinterpret_cast<char*>(Shape.errorBlob->GetBufferPointer()));
		assert(false);
	}

	//�o�C�i�������ɐ���

	hr = device->CreateRootSignature(0, Shape.signatureBlob->GetBufferPointer(),
		Shape.signatureBlob->GetBufferSize(), IID_PPV_ARGS(&Shape.rootSignature));
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

	graphicsPipelineStateDesc.pRootSignature = Shape.rootSignature; //RootSignature
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
		IID_PPV_ARGS(&Shape.GraphicsPipelineState));
	assert(SUCCEEDED(hr));



}

void Model::ShaderRelease()
{
	shader.pixelBlob->Release();
	shader.vertexBlob->Release();
}


ID3D12Resource* Model::CreateBufferResource(ID3D12Device* device, size_t sizeInbyte)
{
	ID3D12Resource* RssultResource;
	//���_���\�[�X�p�̃q�[�v�̐ݒ�
	D3D12_HEAP_PROPERTIES uploadHeapProperties{};

	uploadHeapProperties.Type = D3D12_HEAP_TYPE_UPLOAD; //UploadHeap���g��

	//���_���\�[�X�̐ݒ�
	D3D12_RESOURCE_DESC ResourceDesc{};


	//�o�b�t�@���\�[�X�B�e�N�X�`���̏ꍇ�͂܂��ʂ̐ݒ������
	ResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	ResourceDesc.Width = sizeInbyte; //���\�[�X�̃T�C�Y�B�����vector4��3���_��

	//�o�b�t�@�̏ꍇ�͂�����1�ɂ��錈�܂�
	ResourceDesc.Height = 1;
	ResourceDesc.DepthOrArraySize = 1;
	ResourceDesc.MipLevels = 1;
	ResourceDesc.SampleDesc.Count = 1;

	//�o�b�t�@�̏ꍇ�͂���ɂ��錈�܂�
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

	//�g�p���郊�\�[�X�̃T�C�Y�͒��_3���̃T�C�Y
	resultBufferView.SizeInBytes = UINT(sizeInbyte);

	//1���_������̃T�C�Y
	resultBufferView.StrideInBytes = UINT(sizeInbyte / 3);
	return resultBufferView;

}

ResourcePeroperty  Model::CreateResource()
{
	ResourcePeroperty resultResource;

	resultResource.Vertex = CreateBufferResource(device, sizeof(Vector4) * 3);
	resultResource.Material = CreateBufferResource(device, sizeof(Vector4));
	resultResource.wvpResource = CreateBufferResource(device, sizeof(Matrix4x4));
	resultResource.BufferView = CreateBufferView(sizeof(Vector4) * 3,resultResource.Vertex);
   
	return resultResource;

}

Vector4 Model::ColorAdapter(unsigned int color)
{
	Vector4 ResultColor = {

	   ((color >> 24) & 0xFF) / 255.0f, // ��

	   ((color >> 16) & 0xFF) / 255.0f, // ��

	   ((color >> 8) & 0xFF) / 255.0f,  // ��

	   ((color) & 0xFF) / 255.0f //�����x

	};

	return ResultColor;

}

void Model::ShapeDraw(Position position, unsigned int ColorCode, Matrix4x4 worldTransform,ResourcePeroperty Resource)
{
	Vector4* vertexData = nullptr;
	Vector4* MaterialData = nullptr;
	Matrix4x4* wvpData = nullptr;
	//�������ނ��߂̃A�h���X���擾
	Resource.Vertex->Map(0, nullptr, reinterpret_cast<void**>(&vertexData));
	Resource.Material->Map(0, nullptr, reinterpret_cast<void**>(&MaterialData));
	Resource.wvpResource->Map(0, nullptr, reinterpret_cast<void**>(&wvpData));

	

	//���W
	//����
	vertexData[0] = { position.left.x,position.left.y,position.left.z , 1.0f};

	//��
	vertexData[1] = { position.top.x,position.top.y,position.top.z,1.0f};

	//�E��
	vertexData[2] = { position.right.x,position.right.y,position.right.z,1.0f};

	//�}�e���A��
	Vector4 colorData = ColorAdapter(ColorCode);

	*MaterialData = colorData;

	//�s��̕ϊ�
	
	*wvpData = worldTransform;


	ShapeDrawCommands(commands,Resource,Shape);

}

void Model::ShapeDrawCommands(Commands commands, ResourcePeroperty Resource,PSOProperty Shape)
{

	commands.List->SetGraphicsRootSignature(Shape.rootSignature);
	commands.List->SetPipelineState(Shape.GraphicsPipelineState);//

	commands.List->IASetVertexBuffers(0, 1, &Resource.BufferView);

	//�`���ݒ�BPSO�ɐݒ肵�Ă�����̂Ƃ͂܂��ʁB�������̂�ݒ肷��ƍl���Ă����Ηǂ�
	commands.List->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//�}�e���A��CBuffer�̏ꏊ��ݒ�
	commands.List->SetGraphicsRootConstantBufferView(0, Resource.Material->GetGPUVirtualAddress());

	//wvp�p��CBuffer�̏ꏊ��ݒ�
	commands.List->SetGraphicsRootConstantBufferView(1, Resource.wvpResource->GetGPUVirtualAddress());

	//�`��(DrawCall/�h���[�R�[��)�B
	commands.List->DrawInstanced(3, 1, 0, 0);


}
