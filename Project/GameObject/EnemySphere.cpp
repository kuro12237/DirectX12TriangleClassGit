#include "EnemySphere.h"

Enemy::Enemy()
{
}

Enemy::~Enemy()
{
}

void Enemy::Initialize()
{
	model = make_unique<Model>();
	model->Initialize(new ModelSphereState);
	texHandle_ = TextureManager::LoadTexture("Resources/uvChecker.png");
	model->SetTexHandle(texHandle_);
	model->SetColor({ 0, 1, 0, 1 });
	worldTransform_.translate = { -5,0,0 };

	SetCollosionAttribute(kCollisionAttributeEnemy);
	SetCollisionMask(kCollisionAttributePlayer);
}

void Enemy::Update()
{
	model->SetColor({ 0, 0, 1, 1 });
	worldTransform_.UpdateMatrix();
}

void Enemy::Draw(ViewProjection view)
{
	model->Draw(worldTransform_, view);
}

Vector3 Enemy::GetWorldPosition()
{
	Vector3 worldPos;

	worldPos.x = worldTransform_.matWorld.m[3][0];
	worldPos.y = worldTransform_.matWorld.m[3][1];
	worldPos.z = worldTransform_.matWorld.m[3][2];
	return worldPos;

}

void Enemy::OnCollision()
{
	model->SetColor({ 1,0,0,1 });
}
