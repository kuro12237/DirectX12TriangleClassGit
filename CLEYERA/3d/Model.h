#pragma once
#include"WorldTransform.h"



class Model
{
public:
	Model();
	~Model();

	static Model* GetInstance();

	static void Initialize();


	static void Draw();
private:

	WorldTransform worldTransform_;

};



