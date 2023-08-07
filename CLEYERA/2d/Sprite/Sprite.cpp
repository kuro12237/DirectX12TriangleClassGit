#include "Sprite.h"

Sprite::Sprite()
{
}

Sprite::~Sprite()
{
}



void Sprite::Initialize(Vector2 leftpos, float size,WorldTransform worldTransform, texResourceProperty texResource,const SpriteMode mode)
{
	worldTransform_ = worldTransform;
	tex_ = texResource;
	const int TriangleNumVertex = 3;
	const int BoxNumVertex = 6;

	pos_.leftTop = { leftpos.x,leftpos.y,0.0f,1.0f };
	pos_.rightTop = { leftpos.x + size,leftpos.y,0.0f,1.0f };
	pos_.leftBottom = { leftpos.x,leftpos.y + size,0.0f,1.0f };
	pos_.rightBottom = { leftpos.x + size,leftpos.y + size,0.0f,1.0f };

	switch (mode)
	{
	case Triangle:

	
		mode_ = mode;
		resource_ = CreateResource(TriangleNumVertex);
	
		break;

	case Box:

		resource_ = CreateResource(BoxNumVertex);
		mode_ = mode;
		
		break;
	}


}

void Sprite::TransferMatrix(Matrix4x4 m)
{
	worldTransform_.matWorld = m;
}

void Sprite::SetTexProperty(texResourceProperty NewTex)
{
	tex_ = NewTex;
}

void Sprite::Draw()
{
	VertexData* vertexData = nullptr;
	Vector4* MaterialData = nullptr;
	Matrix4x4* wvpData = nullptr;


	const int TriangleNum = 3;
	const int BoxNum = 6;

	switch (mode_)
	{
	case Triangle:
		//�������ނ��߂̃A�h���X���擾
		resource_.Vertex->Map(0, nullptr, reinterpret_cast<void**>(&vertexData));
		resource_.Material->Map(0, nullptr, reinterpret_cast<void**>(&MaterialData));
		resource_.wvpResource->Map(0, nullptr, reinterpret_cast<void**>(&wvpData));

		vertexData[0].position = { pos_.leftBottom};

		vertexData[0].texcoord = { 0.0f,1.0f };
		////��
		vertexData[1].position = { pos_.leftTop };
		vertexData[1].texcoord = { 0.0f,0.0f };
		////�E
		vertexData[2].position = { pos_.rightBottom };
		vertexData[2].texcoord = { 1.0f,1.0f };

		//�}�e���A��

		*MaterialData = color_;

		//�s��̕ϊ�

		worldTransform_.matWorld = Camera::worldOthographicMatrix(worldTransform_.matWorld);

		*wvpData = worldTransform_.matWorld;

		CommandCall(TriangleNum);
		break;

	case Box:

		//�������ނ��߂̃A�h���X���擾
		resource_.Vertex->Map(0, nullptr, reinterpret_cast<void**>(&vertexData));
		resource_.Material->Map(0, nullptr, reinterpret_cast<void**>(&MaterialData));
		resource_.wvpResource->Map(0, nullptr, reinterpret_cast<void**>(&wvpData));
		///�ꖇ��
		//����
		vertexData[0].position = { pos_.leftBottom };
	
		vertexData[0].texcoord = { 0.0f,1.0f };
		//����
		vertexData[1].position = { pos_.leftTop };
	
		vertexData[1].texcoord = { 0.0f,0.0f };
		//����
		vertexData[2].position = {pos_.rightBottom};

		vertexData[2].texcoord = { 1.0f,1.0f };
		///�񖇖�
		//����
		vertexData[3].position = {pos_.leftTop};

		vertexData[3].texcoord = { 0.0f,0.0f };
		//����
		vertexData[4].position = {pos_.rightTop};
	
		vertexData[4].texcoord = { 1.0f,0.0f };
		//����
		vertexData[5].position = {pos_.rightBottom};
	
		vertexData[5].texcoord = { 1.0f,1.0f };

		//�}�e���A��

		*MaterialData = color_;

		worldTransform_.matWorld=Camera::worldOthographicMatrix(worldTransform_.matWorld);

		*wvpData = worldTransform_.matWorld;

		CommandCall(BoxNum);

		break;

	}

}

void Sprite::Release()
{
	Sprite::Releace(resource_.Material);
	Sprite::Releace(resource_.Vertex);
	Sprite::Releace(resource_.wvpResource);
}

void Sprite::Releace(ID3D12Resource* resource)
{
	resource->Release();
}

void Sprite::CommandCall(const int Num)
{
	Commands commands = DirectXCommon::GetInstance()->GetCommands();
	PSOProperty PSO = GraphicsPipeline::GetInstance()->GetPSO().sprite;

	commands.List->SetGraphicsRootSignature(
	                        PSO.rootSignature);

	commands.List->SetPipelineState(
		PSO.GraphicsPipelineState);//

	commands.List->IASetVertexBuffers(0, 1, &resource_.BufferView);

	//�`���ݒ�BPSO�ɐݒ肵�Ă�����̂Ƃ͂܂��ʁB�������̂�ݒ肷��ƍl���Ă����Ηǂ�
	commands.List->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);


	//wvp�p��CBuffer�̏ꏊ��ݒ�
	commands.List->SetGraphicsRootConstantBufferView(1, resource_.wvpResource->GetGPUVirtualAddress());

	//�}�e���A��CBuffer�̏ꏊ��ݒ�
	commands.List->SetGraphicsRootConstantBufferView(0, resource_.Material->GetGPUVirtualAddress());

	//
	commands.List->SetGraphicsRootDescriptorTable(2, tex_.SrvHandleGPU);


	//�`��(DrawCall/�h���[�R�[��)�B
	commands.List->DrawInstanced(Num, 1, 0, 0);

}


ResourcePeroperty Sprite::CreateResource(const int NumVertex)
{
	 ResourcePeroperty result;
	
	 result.Vertex = CreateBufferResource(sizeof(VertexData) * NumVertex);
	 result.Material = CreateBufferResource(sizeof(Vector4));
	 result.wvpResource = CreateBufferResource(sizeof(Matrix4x4));
	 result.BufferView = CreateBufferView(sizeof(VertexData) * NumVertex, result.Vertex, NumVertex);

	 return result;
}
ID3D12Resource* Sprite::CreateBufferResource(size_t sizeInbyte)
{
	ID3D12Device* device = DirectXCommon::GetInstance()->GetDevice();

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
D3D12_VERTEX_BUFFER_VIEW Sprite::CreateBufferView(size_t sizeInbyte, ID3D12Resource* Resource,const int size)
{
	D3D12_VERTEX_BUFFER_VIEW resultBufferView = {};

	resultBufferView.BufferLocation = Resource->GetGPUVirtualAddress();

	//�g�p���郊�\�[�X�̃T�C�Y�͒��_3���̃T�C�Y
	resultBufferView.SizeInBytes = UINT(sizeInbyte);

	//1���_������̃T�C�Y
	resultBufferView.StrideInBytes = UINT(sizeInbyte / size);
	return resultBufferView;
}
