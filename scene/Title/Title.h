#pragma once
#include"../GameSceneManager/GameSceneManager.h"
#include"../GameBase/GameBase.h"

#include"../../CLEYERA/Matrix/MatrixTransform.h"
#include"../../CLEYERA/Vector/VectorTransform.h"
#include"../../CLEYERA/CLEYERA.h"

#define TriangleMax 3

class Title :public GameBase
{
public:

	void Initialize()override;

	void Update()override;

	void Draw()override;

	void Release()override;


private:


};
