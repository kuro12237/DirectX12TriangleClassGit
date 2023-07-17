#pragma once
#include"../GameScene/GameScene.h"
#include"../../Game/Game.h"

#include"../../CLEYERA/Matrix/MatrixTransform.h"
#include"../../CLEYERA/Vector/VectorTransform.h"
#include"../../CLEYERA/CLEYERA.h"

#define TriangleMax 3

class Title :public GameScene
{
public:

	void Initialize(Game* game)override;

	void Update(Game* game)override;

	void Draw(Game* game)override;

	void Release(Game* game)override;


private:


};
