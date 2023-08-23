#include"CreateResources.h"

CreateResources* CreateResources::GetInstance()
{
	static CreateResources instance;
	return &instance;
}

ResourcePeroperty CreateResources::Vector4CreateResource(const int Num)
{
	ResourcePeroperty result = {};
	
	result.Vertex = CreateBufferResource(sizeof(Vector4) * Num);
	result.Material = CreateBufferResource(sizeof(Vector4)*3);
	result.wvpResource = CreateBufferResource(sizeof(Matrix4x4));
	result.BufferView = VertexCreateBufferView(sizeof(Vector4) * Num, result.Vertex, Num);

	return result;
}

ResourcePeroperty CreateResources::VertexDataCreateResource(const int Num)
{
	ResourcePeroperty result = {};

	result.Vertex = CreateBufferResource(sizeof(VertexData) * Num);
	result.Material = CreateBufferResource(sizeof(Vector4) * 3);
	result.Light = CreateBufferResource(sizeof(LightData));
	result.wvpResource = CreateBufferResource(sizeof(TransformationMatrix));
	result.BufferView = VertexCreateBufferView(sizeof(VertexData) * Num, result.Vertex, Num);

	return result;
}

void CreateResources::Release(ID3D12Resource* resource)
{
	
	resource->Release();

}

ID3D12Resource* CreateResources::CreateBufferResource(size_t sizeInbyte)
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

D3D12_VERTEX_BUFFER_VIEW CreateResources::VertexCreateBufferView(size_t sizeInbyte, ID3D12Resource* Resource, int size)
{

	D3D12_VERTEX_BUFFER_VIEW resultBufferView = {};

	resultBufferView.BufferLocation = Resource->GetGPUVirtualAddress();

	//�g�p���郊�\�[�X�̃T�C�Y�͒��_3���̃T�C�Y
	resultBufferView.SizeInBytes = UINT(sizeInbyte);

	//1���_������̃T�C�Y
	resultBufferView.StrideInBytes = UINT(sizeInbyte / size);
	return resultBufferView;
}

D3D12_INDEX_BUFFER_VIEW CreateResources::IndexCreateBufferView(size_t sizeInbyte, ID3D12Resource* Resource)
{
	D3D12_INDEX_BUFFER_VIEW resultBufferView = {};

	resultBufferView.BufferLocation = Resource->GetGPUVirtualAddress();

	//�g�p���郊�\�[�X�̃T�C�Y�͒��_3���̃T�C�Y
	resultBufferView.SizeInBytes = UINT(sizeInbyte);

	//1���_������̃T�C�Y
	resultBufferView.Format = DXGI_FORMAT_R32_UINT;

	return resultBufferView;
}
