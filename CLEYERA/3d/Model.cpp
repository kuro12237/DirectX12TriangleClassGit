#include"Model.h"

Model::Model()
{
}

Model::~Model()
{
}

void Model::Initialize(Vector4 pos,float size, WorldTransform worldTransform, texResourceProperty tex,SlectModel select)
{

	switch (select)
	{
	case Cube:

		break;
	case Sphere:
		state_ = new StateSphere;

		break;
	}

	state_->Initialize(pos, size, worldTransform, tex);

}

void Model::Draw()
{
	state_->Draw();
}

void Model::Release()
{
	state_->Release();
}

void Model::TransferMatrix(Matrix4x4 m)
{
	state_->TransferMatrix(m);
}

void Model::SetTexProperty(texResourceProperty NewTex)
{
	state_->SetTexProperty(NewTex);
}
