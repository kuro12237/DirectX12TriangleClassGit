#pragma once
#include"../Game/GameScene.h"
#include"../GameBase/GameBase.h"
#include"../../CLEYERA/Matrix/MatrixTransform.h"
#include"../../CLEYERA/Vector/VectorTransform.h"
#include"../../CLEYERA/CLEYERA.h"
#include"../Title/Title.h"


#define TriangleMax 3

class EngineDebug:public GameBase
{
public:

	void Initialize(GameScene* game)override;

	void Update(GameScene* game)override;

	void Draw(GameScene* game)override;

	void Release(GameScene* game)override;


private:

	MatrixTransform* matrixTransform_=nullptr;

	Triangle triangle[TriangleMax];
	
	texResourceProperty texHandule;
	int StateTimer = 300;
};

