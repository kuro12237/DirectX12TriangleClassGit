#pragma once
#include"WorldTransform.h"
#include"GraphicsPipeline.h"


class Model
{
public:
	Model();
	~Model();

	static Model* GetInstance();

	static void CompileShader();

	void Initialize();

	void Draw();
private:

	WorldTransform worldTransform_;

	Shaders shape_ = { nullptr,nullptr };

};



