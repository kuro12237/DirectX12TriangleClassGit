#include "DebugScene.h"

void DebugScene::Initialize(GameManager* Scene)
{
	input_ = Input::GetInstance();

	model_ = make_unique< Model>();
	//sprite_ = make_unique<Sprite>();

	//texHandle = TextureManager::LoadTexture("Resources/uvChecker.png");
	//BlockTexHandle = TextureManager::LoadTexture("Resources/block.png");
	model_->Initialize(new ModelSphereState);
	//model_->CreateFromObj("Axis");
	//sprite_->Initialize(new SpriteBoxState, { 0,0 }, { 500,200 });
	//sprite_->SetTexHandle(BlockTexHandle);
	//sprite_->SetColor({ 1,1,1,1.0f });

	SpriteWorldTransform = {};
	SpriteWorldTransform.Initialize();

	worldTransform = {};
	worldTransform.Initialize();

	viewProjection.Initialize({ 0.2f,-0.6f,0.0f }, { 11.0f,5.0f,-15 });

	soundHandle_ = Audio::SoundLoadWave("Resources/Select.wav");
	Scene;
}

void DebugScene::Update(GameManager* Scene)
{
	ImGui::Begin("Debugcamera");
	ImGui::SliderFloat3("trans", &viewProjection.translation_.x, -15, 15);
	ImGui::SliderFloat3("rotate", &viewProjection.rotation_.x, -1, 1);
	ImGui::End();


	ImGui::Begin("Sprite");
	ImGui::DragFloat3("scale", &SpriteWorldTransform.scale.x, 0.1f);
	ImGui::DragFloat3("rotate", &SpriteWorldTransform.rotation.x, 0.1f);
	ImGui::DragFloat3("trans", &SpriteWorldTransform.translate.x, 0.1f);
	ImGui::End();

	ImGui::Begin("Axis");
	ImGui::DragFloat3("scale", &worldTransform.scale.x, 0.1f);
	ImGui::DragFloat3("rotate", &worldTransform.rotation.x, 0.1f);
	ImGui::DragFloat3("trans", &worldTransform.translate.x, 0.1f);
	ImGui::Text("uv");
	ImGui::DragFloat3("uvscale", &uvScale.x, 0.1f);
	ImGui::DragFloat3("uvrotate", &uvrotate.x, 0.1f);
	ImGui::DragFloat3("uvTrans", &uvtranslate.x, 0.1f);
	ImGui::End();


	ImGui::Begin("key");
	ImGui::Text("0 Pushkey PlayAudio");
	ImGui::Text("9 Pushkey StateChange");
	ImGui::Text("8 Pushkey ObjRotate");
	ImGui::End();


	//model_->SetUvRotate(uvrotate);
	//model_->SetUvScale(uvScale);
	//model_->SetUvTranslate(uvtranslate);

	worldTransform.UpdateMatrix();
	viewProjection.UpdateMatrix();
	SpriteWorldTransform.UpdateMatrix();

	
	if (input_->PushKey(DIK_8))
	{
		worldTransform.rotation.y += 0.1f;
	}

	if (input_->PushKeyPressed(DIK_9))
	{
		Scene->ChangeState(new GameScene);
	}
	if (input_->PushKeyPressed(DIK_0))
	{
		worldTransform.rotation.y+=0.1f;
		Audio::AudioPlayWave(soundHandle_);
	}
	
	Scene;
}

void DebugScene::Draw(GameManager* Scene)
{
	DebugTools::SetViewProjection(viewProjection);
	//viewProjection = DebugCamera::DebugCameraViewProjection();
	

	model_->Draw(worldTransform, viewProjection);
	//sprite_->Draw(SpriteWorldTransform);
	Scene;
}
