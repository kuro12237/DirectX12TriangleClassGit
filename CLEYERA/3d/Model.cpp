#include"Model.h"

Model::Model()
{
}

Model::~Model()
{
}

Model* Model::GetInstance()
{
	static Model instance;

	return &instance;
}

void Model::CompileShader()
{


}

void Model::Initialize()
{
}
