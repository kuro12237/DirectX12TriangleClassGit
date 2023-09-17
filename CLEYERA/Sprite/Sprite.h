#pragma once
#include"Pch.h"
#include"WorldTransform.h"
#include"Sprite/state/SpriteBoxState.h"

class Sprite
{
public:
	Sprite() {};
	~Sprite() { delete state_; };

	void Initialize(ISpriteState* state,Vector2 pos,Vector2 size);

	void Draw(WorldTransform worldTransform);

#pragma region Set
	void SetTexHandle(uint32_t texHandle) { texHandle_ = texHandle; }
	void SetUvScale(Vector3 uvScale) { uvScale_ = uvScale; }
	void SetUvRotate(Vector3 uvRotate) { uvRotate_ = uvRotate; }
	void SetUvTranslate(Vector3 uvTranslate) { uvTranslate_ = uvTranslate; }
	void SetColor(Vector4 color) { color_ = color; }
#pragma endregion 

#pragma region get
	WorldTransform GetWorldTransform() { return worldTransform_; }
	Vector4 GetColor() { return color_; }
	uint32_t GetTexHandle() { return texHandle_; }

	Vector3 GetuvScale() { return uvScale_; }
	Vector3 GetuvRotate() { return uvRotate_; }
	Vector3 GetuvTranslate() { return uvTranslate_; }


	/// <summary>
	/// GetSpritePos
	/// </summary>
	Vector2 GetPos() { return Pos_; }
	/// <summary>
	/// GetSpriteSize
	/// </summary>
	Vector2 GetSize() { return size_; }

	/// <summary>
	/// 色コードをVector4に変換関数
	/// </summary>
	static Vector4 ColorConversion(uint32_t rgbaValue);

#pragma endregion

private:
	Vector2 Pos_ = { 0,0};
	Vector2 size_ = {};


	Vector4 color_ = { 1,1,1,1 };
	WorldTransform worldTransform_ = {};
	uint32_t texHandle_ = 0;

	Vector3 uvScale_ = { 1,1,1 };
	Vector3 uvRotate_ = { 0,0,0 };
	Vector3 uvTranslate_ = { 0,0,0 };

	ISpriteState* state_ = {};
};


