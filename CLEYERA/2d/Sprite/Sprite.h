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


	void Initialize(Vector4 pos, texResourceProperty texResource,const SpriteMode mode);

	void TranferMatrix(Matrix4x4 m);

	void SetTexProperty(texResourceProperty NewTex);

	void Draw();

private:


	/// <summary>
	/// Resource���쐬
	/// </summary>
	/// <param name="device"></param>
	/// <param name="sizeInbyte"></param>
	/// <returns></returns>
	static ID3D12Resource* CreateBufferResource( size_t sizeInbyte);

	/// <summary>
	/// BufferView���쐬
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

	SpriteMode mode_;
};
