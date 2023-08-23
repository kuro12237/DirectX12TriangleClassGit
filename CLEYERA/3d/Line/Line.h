#pragma once
#include"CreateResources/CreateResources.h"
#include"WorldTransform.h"

struct  LinePloperty
{
	Vector4 startPos;
	Vector4 endpos;
	Vector4 color;
};


class Line
{
public:
	Line();
	~Line();

	void Initialize(LinePloperty line, WorldTransform worldTransform);

	void Draw();

	void Release();

	void SetMatrix(Matrix4x4 m) { worldTransform_.matWorld = m; }
private:

	void DrawCall(const int IndexNum);

	LinePloperty line_ = {};
	Matrix4x4 matworld = {};
	
	ResourcePeroperty resource_ = {};

	WorldTransform worldTransform_ = {};


};

