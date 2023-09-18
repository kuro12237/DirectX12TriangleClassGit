#pragma once
#include"IScene.h"
#include"GameManager.h"

class GameScene : public IScene
{
public:
	GameScene() {};
	~GameScene() {};

	void Initialize(GameManager* scene)override;

	void Update(GameManager* scene)override;

	void Draw(GameManager* scene)override;

private:
	ViewProjection viewProjection{};
};

