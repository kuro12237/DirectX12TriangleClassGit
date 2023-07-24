#pragma once
#include"../Game/GameScene.h"
#include"../GameBase/GameBase.h"

#include"../../CLEYERA/Matrix/MatrixTransform.h"
#include"../../CLEYERA/Vector/VectorTransform.h"
#include"../../CLEYERA/CLEYERA.h"

#define TriangleMax 3

class Title :public GameBase
{
public:

	void Initialize(GameScene* game)override;

	void Update(GameScene* game)override;

	void Draw(GameScene* game)override;

	void Release(GameScene* game)override;


private:


};
