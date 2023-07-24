#pragma once
#include"../GameSceneManager/GameSceneManager.h"
#include"../GameBase/GameBase.h"
#include"../../CLEYERA/Matrix/MatrixTransform.h"
#include"../../CLEYERA/Vector/VectorTransform.h"
#include"../../CLEYERA/CLEYERA.h"
#include"../Title/Title.h"
#include"../../CLEYERA/WorldTransform/WorldTransform.h"

#define TRIANGLE_MAX 2

class EngineDebug:public GameBase
{
public:

	void Initialize()override;

	void Update()override;

	void Draw()override;

	void Release()override;


private:

	MatrixTransform* matrixTransform_=nullptr;

	Vector3 pos_;
	ResourcePeroperty resource_;
	unsigned int color_;
	WorldTransform worldTransform_;

	
	int StateTimer = 300;
};

