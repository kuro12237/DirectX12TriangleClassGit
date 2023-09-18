#pragma once
#include"IScene.h"
#include"GameManager.h"
#include"Cleyera.h"

#include"GameScene.h"
#include"Input.h"
#include"Audio.h"

class DebugScene:public IScene
{
public:
	DebugScene() {};
	~DebugScene() {};

	void Initialize(GameManager* Scene)override;

	void Update(GameManager* Scene)override;

	void Draw(GameManager* Scene)override;




private:

	Input* input_ = nullptr;

	unique_ptr<Model> model_ = nullptr;
	unique_ptr<Sprite>sprite_ = nullptr;
	uint32_t BlockTexHandle = {};
	uint32_t texHandle = {};

	WorldTransform SpriteWorldTransform = {};
	WorldTransform worldTransform = {};
	ViewProjection viewProjection{};

	Vector3 uvScale = { 1,1,1, };
	Vector3 uvrotate = {};
	Vector3 uvtranslate = {};
	
	int32_t ChangeSceneTimer_ = 0;

	uint32_t soundHandle_  = 0;
};
