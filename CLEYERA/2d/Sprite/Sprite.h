#pragma once
#include"Mesh.h"
#include"TexManager.h"
#include"Camera.h"
#include"CreateResources/CreateResources.h"

enum SpriteMode
{
   Triangle,
   Box
};
struct BoxPosition
{
	Vector4 leftTop;
	Vector4 rightTop;
	Vector4 leftBottom;
	Vector4 rightBottom;

};

class Sprite
{
public:
	Sprite();
	~Sprite();


	void Initialize(Vector2 leftpos,float size,WorldTransform worldTransform, texResourceProperty texResource,const SpriteMode mode);

	void TransferMatrix(Matrix4x4 m);

	void SetTexProperty(texResourceProperty NewTex);

	Matrix4x4 GetWorldTransform() { return worldTransform_.matWorld; }
	void Draw();

	void Release();
private:

	 void CommandCall(const int Num);

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
	
	texResourceProperty tex_;
	ResourcePeroperty resource_;

	Vector4 color_ = {1,1,1,1};

	
	BoxPosition pos_;

	SpriteMode mode_;
};
