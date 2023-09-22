#pragma once
#include"IScene.h"
#include"GameManager.h"
#include"Cleyera.h"

#include"GameScene.h"
#include"Input.h"
#include"Audio.h"
#include"FileLoader.h"

#include"GameObject/PlayerSphere.h"
#include"GameObject/EnemySphere.h"
#include"CollisionManager.h"

class DebugScene:public IScene
{
public:
	DebugScene() {};
	~DebugScene();


	void Initialize(GameManager* Scene)override;

	void Update(GameManager* Scene)override;

	void Draw(GameManager* Scene)override;

private:
	void TestCsvFile();

	void CheckAllCollision();
	
	ViewProjection viewProjection{};

	uint32_t soundHandle_  = 0;
	stringstream file{};


	unique_ptr<Player>player_ = nullptr;
	unique_ptr<Enemy>enemy_=nullptr;
	unique_ptr <CollisionManager> collisionManager = nullptr;
};
