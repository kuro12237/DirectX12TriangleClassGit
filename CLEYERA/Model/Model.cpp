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

void Model::CreateFromObj(const string& directoryFile)
{
	state_ = new ModelObjState();
    directoryPath_ = directoryFile;
	state_->Initialize(this);
}
void Model::Draw(WorldTransform worldTransform, ViewProjection viewprojection)
{
	worldTransform_ = worldTransform;
	state_->Draw(this, worldTransform_,viewprojection);
}

void Model::UseLight(SUseLight use)
{
	uselight_ = use;
}

Vector4 Model::ColorConversion(uint32_t rgbaValue)
{
	Vector4 result = {};
	result.x = ((rgbaValue >> 24) & 0xFF) / 255.0f; // Red
	result.y = ((rgbaValue >> 16) & 0xFF) / 255.0f; // Green
	result.z = ((rgbaValue >> 8) & 0xFF) / 255.0f;  // Blue
	result.w = (rgbaValue & 0xFF) / 255.0f;         // Alpha
	return result;
}

