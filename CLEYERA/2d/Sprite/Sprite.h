#pragma once
#include"Mesh.h"
#include"TexManager.h"
#include"Camera.h"
#include"CreateResources/CreateResources.h"


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


	void Initialize(Vector2 leftpos,float size,WorldTransform worldTransform, texResourceProperty texResource);
	
	void Draw();

	void Release();

	void TransferMatrix(Matrix4x4 m);

	void SetTexProperty(texResourceProperty NewTex);

	Matrix4x4 GetWorldTransform() { return worldTransform_.matWorld; }

private:

	void CommandCall(const int Num);

    static void Releace(ID3D12Resource *resource);

	ResourcePeroperty CreateResource(const int NumVertex, const int NumIndex);


	WorldTransform worldTransform_ = {};
	texResourceProperty tex_ = {};
	ResourcePeroperty resource_ = {};
	Vector4 color_ = {1,1,1,1};
	BoxPosition pos_ = {};

};
