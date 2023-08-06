#pragma once
#include"Mesh.h"

#include"TexManager.h"

enum SpriteMode
{
   Triangle,
   Box
};


class Sprite
{
public:
	Sprite();
	~Sprite();


	void Initialize(Vector4 pos, WorldTransform worldTransform, texResourceProperty texResource,const SpriteMode mode);

	void TransferMatrix(Matrix4x4 m);

	void SetTexProperty(texResourceProperty NewTex);

	void Draw();

	void Release();
private:

	 void CommandCall();

	 static void Releace(ID3D12Resource *resource);

	/// <summary>
	/// ResourceÇçÏê¨
	/// </summary>
	/// <param name="device"></param>
	/// <param name="sizeInbyte"></param>
	/// <returns></returns>
	static ID3D12Resource* CreateBufferResource( size_t sizeInbyte);

	/// <summary>
	/// BufferViewÇçÏê¨
	/// </summary>
	/// <param name="sizeInbyte"></param>
	/// <param name="Resource"></param>
	/// <returns></returns>
	static D3D12_VERTEX_BUFFER_VIEW CreateBufferView(size_t sizeInbyte, ID3D12Resource* Resource,const int size);


	ResourcePeroperty CreateResource(const int NumVertex);

	WorldTransform worldTransform_;
	Vector4 centerPos_;

	texResourceProperty tex_;
	ResourcePeroperty resource_;

	Vector4 color_ = {1,1,1,1};
	float size_ = 0.1f;
	SpriteMode mode_;
};
