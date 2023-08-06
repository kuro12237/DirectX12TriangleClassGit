#include "Sprite.h"

Sprite::Sprite()
{
}

Sprite::~Sprite()
{
}



void Sprite::Initialize(Vector4 pos, texResourceProperty texResource,const SpriteMode mode)
{
	centerPos_ = pos;

	switch (mode)
	{
	case Triangle:

		const int NumVertex = 3;
		mode_ = mode;
		resource_ = CreateResource(NumVertex);
		worldTransform_.Initialize();
		tex_ = texResource;

		break;



	}


}

void Sprite::TranferMatrix(Matrix4x4 m)
{
	worldTransform_.matWorld = m;
}

void Sprite::SetTexProperty(texResourceProperty NewTex)
{
	tex_ = NewTex;
}

void Sprite::Draw()
{
}

ResourcePeroperty Sprite::CreateResource(const int NumVertex)
{
	 ResourcePeroperty result;

	 result.Vertex = CreateBufferResource(sizeof(VertexData) * NumVertex);
	 result.Material = CreateBufferResource(sizeof(Vector4));
	 result.wvpResource = CreateBufferResource(sizeof(Matrix4x4));
	 result.BufferView = CreateBufferView(sizeof(VertexData) * NumVertex * NumVertex, result.Vertex,NumVertex);

	 return result;
}
ID3D12Resource* Sprite::CreateBufferResource(size_t sizeInbyte)
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
D3D12_VERTEX_BUFFER_VIEW Sprite::CreateBufferView(size_t sizeInbyte, ID3D12Resource* Resource,const int size)
{
	D3D12_VERTEX_BUFFER_VIEW resultBufferView;

	resultBufferView.BufferLocation = Resource->GetGPUVirtualAddress();

	//使用するリソースのサイズは頂点3つ分のサイズ
	resultBufferView.SizeInBytes = UINT(sizeInbyte);

	//1頂点あたりのサイズ
	resultBufferView.StrideInBytes = UINT(sizeInbyte / size);
	return resultBufferView;
}
