#pragma once
#include"Model.h"
#include"IModelState.h"



class ModelLineState:public IModelState
{
public:
	~ModelLineState() {};

	void Initialize(Model* state)override;

	void Draw(Model* state, WorldTransform worldTransform, ViewProjection viewprojection)override;

private:

	void CommandCall();

	const int32_t VertexSize= 2;

	ResourcePeroperty resource_ = {};

};

