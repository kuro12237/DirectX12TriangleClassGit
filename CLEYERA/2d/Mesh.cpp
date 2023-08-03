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
	//書き込むためのアドレスを取得
	resource_.Vertex->Map(0, nullptr, reinterpret_cast<void**>(&vertexData));
	resource_.Material->Map(0, nullptr, reinterpret_cast<void**>(&MaterialData));
	resource_.wvpResource->Map(0, nullptr, reinterpret_cast<void**>(&wvpData));

	//座標
	//左下
	vertexData[0] = { CenterPos_.x - size,CenterPos_.y - size,CenterPos_.z,1.0f };

	//上
	vertexData[1] = { CenterPos_.x,CenterPos_.y + size,CenterPos_.z,1.0f };

	//右上
	vertexData[2] = { CenterPos_.x + size,CenterPos_.y - size,CenterPos_.z,1.0f };

	//マテリアル
	Vector4 colorData = Color_;

	*MaterialData = colorData;

	//行列の変換

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

D3D12_VERTEX_BUFFER_VIEW Mesh::CreateBufferView(size_t sizeInbyte, ID3D12Resource* Resource,int size)
{
	D3D12_VERTEX_BUFFER_VIEW resultBufferView;

	resultBufferView.BufferLocation = Resource->GetGPUVirtualAddress();

	//使用するリソースのサイズは頂点3つ分のサイズ
	resultBufferView.SizeInBytes = UINT(sizeInbyte);

	//1頂点あたりのサイズ
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

	//形状を設定。PSOに設定しているものとはまた別。同じものを設定すると考えておけば良い
	commands.List->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//マテリアルCBufferの場所を設定
	commands.List->SetGraphicsRootConstantBufferView(0, resource.Material->GetGPUVirtualAddress());


	//wvp用のCBufferの場所を設定
	commands.List->SetGraphicsRootConstantBufferView(1, resource.wvpResource->GetGPUVirtualAddress());

	//描画(DrawCall/ドローコール)。
	commands.List->DrawInstanced(3, 1, 0, 0);


}

void Mesh::resourceRelease(ID3D12Resource* resource)
{
	resource->Release();
}




