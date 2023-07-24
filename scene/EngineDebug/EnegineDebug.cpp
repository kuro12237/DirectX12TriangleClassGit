#include"EngineDebug.h"

void EngineDebug::Initialize()
{
	
	matrixTransform_ = new MatrixTransform();

	//三角形初期化
	pos_ ={ 0.0f,0.0f,0.0f};
	
	resource_ = Cleyera::CreateShapeResource();
	
	color_ = BLACK;

	worldTransform_.Initiallize();

	worldTransform_.scale_ = { 1,1,1 };

}

void EngineDebug::Update()
{

	StateTimer--;

	if (StateTimer<0)
	{
		//GameScene::ChangeState(new Title());
	}
	worldTransform_.UpdateMatrix();
}

void EngineDebug::Draw()
{

	Cleyera::TriangleDraw(
		pos_,
		color_, 
		worldTransform_,
		resource_
	);
}

void EngineDebug::Release()
{

	Cleyera::TriangleResourceRelease(resource_);

}
