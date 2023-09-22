#include "GameScene.h"

void GameScene::Initialize(GameManager* scene)
{
	viewProjection.Initialize({ 0.2f,-0.6f,0.0f }, { 11.0f,5.0f,-15 });
	scene;
}

void GameScene::Update(GameManager* scene)
{
	ImGui::Begin("ChangeDebugScene");
	ImGui::Text("9 key");
	ImGui::End();

	if (Input::GetInstance()->PushKeyPressed(DIK_9))
	{
		scene->ChangeState(new DebugScene);
	}

	viewProjection.UpdateMatrix();
	viewProjection = DebugTools::ConvertViewProjection(viewProjection);
}

void GameScene::Draw(GameManager* scene)
{
	
	scene;
}
