#pragma once
#include"Model.h"
#include"Collider.h"
#include"ColliderConfig.h"

class Enemy :public Collider
{
public:
	Enemy();
	~Enemy();


	void Initialize();

	void Update();

	void Draw(ViewProjection view);

	Vector3 GetWorldPosition()override;

	void OnCollision()override;
private:

	unique_ptr<Model> model = nullptr;
	uint32_t texHandle_{};
	WorldTransform worldTransform_{};
	Vector4 HitColor = { 1,1,1,1 };
};
