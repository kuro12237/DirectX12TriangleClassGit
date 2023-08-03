#include"Mesh.h"


Mesh::Mesh()
{
}

Mesh::~Mesh()
{
}


Mesh* Mesh::GetInstance()
{
	static Mesh instance;
	return &instance;
}



void Mesh::Initialize(WorldTransform worldTransform,Vector4 pos)
{
	resource_ = Mesh::CreateResource();
	CenterPos_ = pos;
	worldTransform_ = worldTransform;
}

ResourcePeroperty Mesh::CreateResource()
{
	ResourcePeroperty result;
	ID3D12Device* device = DirectXCommon::GetInstance()->GetDevice();

	result.Vertex = CreateBufferResource(device, sizeof(Vector4) * 3);
	result.Material = CreateBufferResource(device, sizeof(Vector4));
	result.wvpResource = CreateBufferResource(device, sizeof(Matrix4x4));
	result.BufferView = CreateBufferView(sizeof(Vector4) * 3, result.Vertex,3);


	return result;

}

void Mesh::Draw()
{
	Vector4* vertexData = nullptr;
	Vector4* MaterialData = nullptr;
	Matrix4x4* wvpData = nullptr;
	//�������ނ��߂̃A�h���X���擾
	resource_.Vertex->Map(0, nullptr, reinterpret_cast<void**>(&vertexData));
	resource_.Material->Map(0, nullptr, reinterpret_cast<void**>(&MaterialData));
	resource_.wvpResource->Map(0, nullptr, reinterpret_cast<void**>(&wvpData));

	//���W
	//����
	vertexData[0] = { CenterPos_.x - size,CenterPos_.y - size,CenterPos_.z,1.0f };

	//��
	vertexData[1] = { CenterPos_.x,CenterPos_.y + size,CenterPos_.z,1.0f };

	//�E��
	vertexData[2] = { CenterPos_.x + size,CenterPos_.y - size,CenterPos_.z,1.0f };

	//�}�e���A��
	Vector4 colorData = Color_;

	*MaterialData = colorData;

	//�s��̕ϊ�

	*wvpData = worldTransform_.matWorld;

	Mesh::ComanndCall(resource_);


}

void Mesh::Release()
{
	Mesh::resourceRelease(resource_.wvpResource);
	Mesh::resourceRelease(resource_.Material);
	Mesh::resourceRelease(resource_.Vertex);
}

ID3D12Resource* Mesh::CreateBufferResource(ID3D12Device* device, size_t sizeInbyte)
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

D3D12_VERTEX_BUFFER_VIEW Mesh::CreateBufferView(size_t sizeInbyte, ID3D12Resource* Resource,int size)
{
	D3D12_VERTEX_BUFFER_VIEW resultBufferView;

	resultBufferView.BufferLocation = Resource->GetGPUVirtualAddress();

	//�g�p���郊�\�[�X�̃T�C�Y�͒��_3���̃T�C�Y
	resultBufferView.SizeInBytes = UINT(sizeInbyte);

	//1���_������̃T�C�Y
	resultBufferView.StrideInBytes = UINT(sizeInbyte / size);
	return resultBufferView;
}

void Mesh::ComanndCall(ResourcePeroperty resource)
{
	Commands commands = DirectXCommon::GetInstance()->GetCommands();
	PSOProperty PSO = GraphicsPipeline::GetInstance()->GetPSO().shape;

	commands.List->SetGraphicsRootSignature(PSO.rootSignature);
	commands.List->SetPipelineState(PSO.GraphicsPipelineState);//

	commands.List->IASetVertexBuffers(0, 1, &resource.BufferView);

	//�`���ݒ�BPSO�ɐݒ肵�Ă�����̂Ƃ͂܂��ʁB�������̂�ݒ肷��ƍl���Ă����Ηǂ�
	commands.List->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//�}�e���A��CBuffer�̏ꏊ��ݒ�
	commands.List->SetGraphicsRootConstantBufferView(0, resource.Material->GetGPUVirtualAddress());


	//wvp�p��CBuffer�̏ꏊ��ݒ�
	commands.List->SetGraphicsRootConstantBufferView(1, resource.wvpResource->GetGPUVirtualAddress());

	//�`��(DrawCall/�h���[�R�[��)�B
	commands.List->DrawInstanced(3, 1, 0, 0);


}

void Mesh::resourceRelease(ID3D12Resource* resource)
{
	resource->Release();
}




