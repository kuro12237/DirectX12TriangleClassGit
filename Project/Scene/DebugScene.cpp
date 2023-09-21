#include "DebugScene.h"

DebugScene::~DebugScene()
{
	Audio::SoundUnLoad(soundHandle_);
	
}

void DebugScene::Initialize(GameManager* Scene)
{
	viewProjection.Initialize({ 0.2f,-0.6f,0.0f }, { 11.0f,5.0f,-15 });

	soundHandle_ = Audio::SoundLoadWave("Resources/Select.wav");
	Scene;
	file = FileLoader::CSVLoadFile("Resources/DebugTest.csv");

	collisionManager = new CollisionManager;

	player_ = make_unique<Player>();
	player_->Initialize();
	enemy_ = make_unique<Enemy>();
	enemy_->Initialize();
}

void DebugScene::Update(GameManager* Scene)
{
	ImGui::Begin("MainCamera");
	ImGui::SliderFloat3("trans", &viewProjection.translation_.x, -15, 15);
	ImGui::SliderFloat3("rotate", &viewProjection.rotation_.x, -1, 1);
	ImGui::End();





	ImGui::Begin("key");
	ImGui::Text("0 Pushkey PlayAudio");
	ImGui::Text("9 Pushkey StateChange");

	ImGui::End();

	viewProjection.UpdateMatrix();



	if (Input::GetInstance()->PushKeyPressed(DIK_9))
	{
		Scene->ChangeState(new GameScene);
	}
	if (Input::GetInstance()->PushKeyPressed(DIK_0))
	{
		Audio::AudioPlayWave(soundHandle_);
	}
	
	player_->Update();
	enemy_->Update();
	Scene;
	viewProjection = DebugTools::ConvertViewProjection(viewProjection);
	CheckAllCollision();
}

void DebugScene::Draw(GameManager* Scene)
{
	player_->Draw(viewProjection);
	enemy_->Draw(viewProjection);

	Scene;
}

void DebugScene::CheckAllCollision()
{
	collisionManager->ClliderClear();
	//
	collisionManager->ClliderPush(player_.get());
	collisionManager->ClliderPush(enemy_.get());
	//
	collisionManager->CheckAllCollision();

}
