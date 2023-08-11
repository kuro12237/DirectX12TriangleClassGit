#pragma once
#include"WorldTransform.h"
#include"GraphicsPipeline.h"
#include"TexManager.h"
#include"3d/CreateResources/CreateResources.h"

class Mesh
{
public:
	Mesh();
	~Mesh();

	static Mesh* GetInstance();

	void Initialize(WorldTransform worldTransform,Vector4 pos,Vector4 Color);
	
	void TransferMatrix(Matrix4x4 m);

	void Draw();

	void Release();

	
private:
	

	/// <summary>
	/// コマンド呼び出し
	/// </summary>
	/// <param name="resource"></param>
	static void ComanndCall(ResourcePeroperty resource);





	ResourcePeroperty resource_;
	WorldTransform worldTransform_;
	Vector4 CenterPos_ = {0.0f,0.0f,0.0f};
	Vector4 Color_ = { 0.0f,0.0f,1.0f,1.0f };
	const float size_ = 0.1f;
};

