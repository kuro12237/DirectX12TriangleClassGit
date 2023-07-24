#include"EngineDebug.h"



void EngineDebug::Initialize(GameScene* game)
{
	game;
	matrixTransform_ = new MatrixTransform();

	//t,l,r
	pos_ ={ 0.0f,0.0f,0.0f};
	
	resource_ = Cleyera::CreateShapeResource();
	
	color_ = BLACK;

	worldTransform_.Initiallize();

	worldTransform_.scale_ = { 10,3,3 };

}

void EngineDebug::Update(GameScene* game)
{
	game;

	

	StateTimer--;
	if (StateTimer<0)
	{
		//Game::ChangeState(new Title());

	}
	worldTransform_.UpdateMatrix();
}

void EngineDebug::Draw(GameScene* game)
{
	game;

	Cleyera::TriangleDraw(
		pos_,
		color_, 
		worldTransform_.matWorld_,
		resource_
	);

	
}

void EngineDebug::Release(GameScene* game)
{
	game;

	Cleyera::TriangleResourceRelease(
		resource_);

}
