#pragma once
#include"WorldTransform.h"
#include"GraphicsPipeline.h"

struct  VertexData
{
	Vector4 position;
	Vector2 texcoord;
};




struct ResourcePeroperty
{
	D3D12_VERTEX_BUFFER_VIEW BufferView;
	ID3D12Resource* Vertex;
	ID3D12Resource* Material;
	ID3D12Resource* wvpResource;
};

class Mesh
{
public:
	Mesh();
	~Mesh();

	static Mesh* GetInstance();

	void Initialize(WorldTransform worldTransform,Vector4 pos);
	
	void TransferMatrix(Matrix4x4 m);

	void Draw();

	void Release();

	
private:
	/// <summary>
	/// ResourceとbufferViewの生成
	/// </summary>
	/// <returns></returns>
	static ResourcePeroperty CreateResource();

	/// <summary>
	/// Resourceを作成
	/// </summary>
	/// <param name="device"></param>
	/// <param name="sizeInbyte"></param>
	/// <returns></returns>
	static ID3D12Resource* CreateBufferResource(ID3D12Device* device, size_t sizeInbyte);

	/// <summary>
	/// BufferViewを作成
	/// </summary>
	/// <param name="sizeInbyte"></param>
	/// <param name="Resource"></param>
	/// <returns></returns>
	static D3D12_VERTEX_BUFFER_VIEW CreateBufferView(size_t sizeInbyte, ID3D12Resource* Resource,int size);

	/// <summary>
	/// コマンド呼び出し
	/// </summary>
	/// <param name="resource"></param>
	static void ComanndCall(ResourcePeroperty resource);

	static void resourceRelease(ID3D12Resource *resource);




	ResourcePeroperty resource_;
	WorldTransform worldTransform_;
	Vector4 CenterPos_ = {0.0f,0.0f,0.0f};
	Vector4 Color_ = { 0.0f,0.0f,1.0f,1.0f };
	const float size = 0.1f;
};

