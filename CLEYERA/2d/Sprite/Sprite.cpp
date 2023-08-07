#include "Sprite.h"

Sprite::Sprite()
{
}

Sprite::~Sprite()
{
}



void Sprite::Initialize(Vector4 pos, WorldTransform worldTransform, texResourceProperty texResource,const SpriteMode mode)
{
	centerPos_ = pos;
	worldTransform_ = worldTransform;
	tex_ = texResource;
	const int TriangleNumVertex = 3;
	const int BoxNumVertex = 6;
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
		//書き込むためのアドレスを取得
		resource_.Vertex->Map(0, nullptr, reinterpret_cast<void**>(&vertexData));
		resource_.Material->Map(0, nullptr, reinterpret_cast<void**>(&MaterialData));
		resource_.wvpResource->Map(0, nullptr, reinterpret_cast<void**>(&wvpData));

		vertexData[0].position = { 
			centerPos_.x-size_, centerPos_.y - size_,
			centerPos_.z,  centerPos_.w
		}; 

		vertexData[0].texcoord = { 0.0f,1.0f };
		////上
		vertexData[1].position = { 
			centerPos_.x , centerPos_.y + size_,
			 centerPos_.z, centerPos_.w 
		};
		vertexData[1].texcoord = { 0.5f,0.0f };
		////右
		vertexData[2].position = {
			centerPos_.x + size_, centerPos_.y - size_,
			centerPos_.z, centerPos_.w 
		};
		vertexData[2].texcoord = { 1.0f,1.0f };

		//マテリアル

		*MaterialData = color_;

		//行列の変換

		*wvpData = worldTransform_.matWorld;

		CommandCall(TriangleNum);
		break;

	case Box:

		//書き込むためのアドレスを取得
		resource_.Vertex->Map(0, nullptr, reinterpret_cast<void**>(&vertexData));
		resource_.Material->Map(0, nullptr, reinterpret_cast<void**>(&MaterialData));
		resource_.wvpResource->Map(0, nullptr, reinterpret_cast<void**>(&wvpData));
		///一枚目
		//←↓
		vertexData[0].position = {
			centerPos_.x - size_, centerPos_.y - size_,
			centerPos_.z,  centerPos_.w
		};
		vertexData[0].texcoord = { 0.0f,1.0f };
		//←↑
		vertexData[1].position = { 
			centerPos_.x- size_ , centerPos_.y + size_,
		    centerPos_.z, centerPos_.w 
		};
		vertexData[1].texcoord = { 0.0f,0.0f };
		//→↓
		vertexData[2].position = { 
			centerPos_.x +size_, centerPos_.y - size_,
			centerPos_.z, centerPos_.w 
		};
		vertexData[2].texcoord = { 1.0f,1.0f };
		///二枚目
		//←↑
		vertexData[3].position = { 
			centerPos_.x - size_, centerPos_.y + size_,
			centerPos_.z, centerPos_.w 
		};
		vertexData[3].texcoord = { 0.0f,0.0f };
		//→↑
		vertexData[4].position = {
			centerPos_.x + size_, centerPos_.y + size_,
			centerPos_.z, centerPos_.w
		};
		vertexData[4].texcoord = { 1.0f,0.0f };
		//→↑
		vertexData[5].position = {
			centerPos_.x + size_, centerPos_.y - size_,
			centerPos_.z, centerPos_.w
		};
		vertexData[5].texcoord = { 1.0f,1.0f };



		//マテリアル

		*MaterialData = color_;

		* wvpData = worldTransform_.matWorld;

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

	//形状を設定。PSOに設定しているものとはまた別。同じものを設定すると考えておけば良い
	commands.List->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//wvp用のCBufferの場所を設定
	commands.List->SetGraphicsRootConstantBufferView(1, resource_.wvpResource->GetGPUVirtualAddress());

	//マテリアルCBufferの場所を設定
	commands.List->SetGraphicsRootConstantBufferView(0, resource_.Material->GetGPUVirtualAddress());


	//
	commands.List->SetGraphicsRootDescriptorTable(2, tex_.SrvHandleGPU);


	//描画(DrawCall/ドローコール)。
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
	D3D12_VERTEX_BUFFER_VIEW resultBufferView = {};

	resultBufferView.BufferLocation = Resource->GetGPUVirtualAddress();

	//使用するリソースのサイズは頂点3つ分のサイズ
	resultBufferView.SizeInBytes = UINT(sizeInbyte);

	//1頂点あたりのサイズ
	resultBufferView.StrideInBytes = UINT(sizeInbyte / size);
	return resultBufferView;
}
