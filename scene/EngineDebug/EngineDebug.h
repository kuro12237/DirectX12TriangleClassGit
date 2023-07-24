#pragma once
#include"../Game/GameScene.h"
#include"../GameBase/GameBase.h"
#include"../../CLEYERA/Matrix/MatrixTransform.h"
#include"../../CLEYERA/Vector/VectorTransform.h"
#include"../../CLEYERA/CLEYERA.h"
#include"../Title/Title.h"
#include"../../CLEYERA/WorldTransform/WorldTransform.h"



class EngineDebug:public GameBase
{
public:

	void Initialize(GameScene* game)override;

	void Update(GameScene* game)override;

	void Draw(GameScene* game)override;

	void Release(GameScene* game)override;


private:

	MatrixTransform* matrixTransform_=nullptr;

	Vector3 pos_;
	ResourcePeroperty resource_;
	unsigned int color_;
	WorldTransform worldTransform_;



	
	int StateTimer = 300;
};

