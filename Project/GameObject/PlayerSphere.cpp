#include "PlayerSphere.h"

Player::Player()
{
}

Player::~Player()
{
}

void Player::Initialize()
{
	model = make_unique<Model>();
	model->UseLight(HARF_LAMBERT);
	model->Initialize(new ModelSphereState);
	texHandle_ = TextureManager::LoadTexture("Resources/uvChecker.png");
	model->SetTexHandle(texHandle_);
	model->SetColor({ 0, 0, 1, 1 });

	SetCollosionAttribute(kCollisionAttributePlayer);
	SetCollisionMask(kCollisionAttributeEnemy);
	
}

void Player::Update()
{
	model->SetColor({ 1, 1, 1, 1 }); 
	const float speed = 0.02f;

	ImGui::Begin("Player");
	ImGui::Text("WASD:Move");
	ImGui::Text("Collision BlueShere");
	ImGui::DragFloat("UP,DOWN", &worldTransform_.translate.y, -0.1f, 0.1f);
	ImGui::End();

	if (Input::GetInstance()->PushKey(DIK_A))
	{
		worldTransform_.translate.x -= speed;
	}
	if (Input::GetInstance()->PushKey(DIK_D))
	{
		worldTransform_.translate.x += speed;
	}
	if (Input::GetInstance()->PushKey(DIK_W))
	{
		worldTransform_.translate.z += speed;
	}
	if (Input::GetInstance()->PushKey(DIK_S))
	{
		worldTransform_.translate.z -= speed;
	}
	worldTransform_.UpdateMatrix();
}

void Player::Draw(ViewProjection view)
{
	model->Draw(worldTransform_, view);
}

Vector3 Player::GetWorldPosition()
{
	Vector3 worldPos;

	worldPos.x = worldTransform_.matWorld.m[3][0];
	worldPos.y = worldTransform_.matWorld.m[3][1];
	worldPos.z = worldTransform_.matWorld.m[3][2];
	return worldPos;

}

void Player::OnCollision()
{
	model->SetColor({ 1,0,0,1 });
}
