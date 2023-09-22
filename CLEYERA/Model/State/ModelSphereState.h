#pragma once
#include"IModelState.h"
#include"Model.h"
#include"CreateResource.h"
#include"WorldTransform.h"
#include"GraphicsPipelineManager.h"

class ModelSphereState :public IModelState
{
public:
	
	~ModelSphereState() {};

	void Initialize(Model* state)override;

	void Draw(Model* state, WorldTransform worldTransform, ViewProjection viewprojection)override;


private:

	void CommandCall(Model*state);

	const int VertexNum = 16;
	ResourcePeroperty resource_ = {};
};

