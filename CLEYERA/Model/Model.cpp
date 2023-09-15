#include "Model.h"


Model::~Model()
{
	delete state_;
}

void Model::Initialize(IModelState* state, Vector4 CenterPos , float size , Vector4 color)
{
	state_ = state;
	state_->Initialize(this);

	CenterPos_ = CenterPos;
	size_=size;
	color_ = color;
}

void Model::Initialize(IModelState* state,Vector4 StartPosition, Vector4 EndPosition, Vector4 Color)
{
	state_ = state;
	StartPos_ = StartPosition;
	EndPos_ = EndPosition;
	color_ = Color;
	state->Initialize(this);

}

void Model::Draw(WorldTransform worldTransform, ViewProjection *viewprojection)
{
	worldTransform_ = worldTransform;
	state_->Draw(this, worldTransform_,viewprojection);
}
