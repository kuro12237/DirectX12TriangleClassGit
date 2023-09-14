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

void Model::Draw(WorldTransform worldTransform)
{
	worldTransform_ = worldTransform;
	state_->Draw(this, worldTransform_);
}
