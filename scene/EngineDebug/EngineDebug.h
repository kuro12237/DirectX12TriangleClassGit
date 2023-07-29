#pragma once
#include"../GameScene/GameScene.h"
#include"../../Game/Game.h"
#include"../../CLEYERA/Matrix/MatrixTransform.h"
#include"../../CLEYERA/Vector/VectorTransform.h"
#include"../../CLEYERA/CLEYERA.h"
#include"../Title/Title.h"


#define TriangleMax 3

enum MODE
{
	Tex,
	Shape
};

class EngineDebug:public GameScene
{
public:

	void Initialize(Game* game)override;

	void Update(Game* game)override;

	void Draw(Game* game)override;

	void Release(Game* game)override;


private:

	MatrixTransform* matrixTransform_=nullptr;

	Triangle triangle[TriangleMax];
	
	texResourceProperty texHandule;
	int StateTimer = 300;

	MODE mode;

};

