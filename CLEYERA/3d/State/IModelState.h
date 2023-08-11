#pragma once
#include"Vector4.h" 
#include"WorldTransform.h"
#include"TexManager.h"

class Model;

class  IModelState
{
public:
	
	virtual void Initialize(
		Vector4 pos, 
		float size,
		WorldTransform worldTransform,
		texResourceProperty tex) = 0;

	virtual void Draw() = 0;

	virtual void Release()=0;

	virtual void TransferMatrix(Matrix4x4 m) = 0;

	virtual void SetTexProperty(texResourceProperty NewTex) = 0;

	virtual Matrix4x4 GetWorldTransform() = 0;


private:

};

