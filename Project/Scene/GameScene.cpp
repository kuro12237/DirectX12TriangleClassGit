#include "GameScene.h"

void GameScene::Initialize(GameManager* scene)
{
	viewProjection.Initialize({ 0.2f,-0.6f,0.0f }, { 11.0f,5.0f,-15 });
	scene;
}

void GameScene::Update(GameManager* scene)
{
	
	viewProjection.UpdateMatrix();
	DebugTools::SetViewProjection(viewProjection);
	scene;
}

void GameScene::Draw(GameManager* scene)
{
	scene;
}
