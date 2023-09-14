#include "Model.h"


Model::~Model()
{
	delete state_;
}

void Model::Initialize(IModelState* state)
{
	state_ = state;
	state_->Initialize(this);
}

void Model::Draw(WorldTransform worldTransform, ViewProjection *viewprojection)
{
	worldTransform_ = worldTransform;
	state_->Draw(this, worldTransform_,viewprojection);
}
