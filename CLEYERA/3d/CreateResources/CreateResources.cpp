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

D3D12_VERTEX_BUFFER_VIEW CreateResources::VertexCreateBufferView(size_t sizeInbyte, ID3D12Resource* Resource, int size)
{

	D3D12_VERTEX_BUFFER_VIEW resultBufferView = {};

	resultBufferView.BufferLocation = Resource->GetGPUVirtualAddress();

	//使用するリソースのサイズは頂点3つ分のサイズ
	resultBufferView.SizeInBytes = UINT(sizeInbyte);

	//1頂点あたりのサイズ
	resultBufferView.StrideInBytes = UINT(sizeInbyte / size);
	return resultBufferView;
}

D3D12_INDEX_BUFFER_VIEW CreateResources::IndexCreateBufferView(size_t sizeInbyte, ID3D12Resource* Resource)
{
	D3D12_INDEX_BUFFER_VIEW resultBufferView = {};

	resultBufferView.BufferLocation = Resource->GetGPUVirtualAddress();

	//使用するリソースのサイズは頂点3つ分のサイズ
	resultBufferView.SizeInBytes = UINT(sizeInbyte);

	//1頂点あたりのサイズ
	resultBufferView.Format = DXGI_FORMAT_R32_UINT;

	return resultBufferView;
}
