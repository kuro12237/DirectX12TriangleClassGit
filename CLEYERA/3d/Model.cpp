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
	Shaders shape;

	shape.vertexBlob =
		GraphicsPipeline::CompilerShader(
			L"Shader/ShapeObject3d.VS.hlsl",
			L"vs_6_0"
		);
	shape.pixelBlob=
		GraphicsPipeline::CompilerShader(
			L"Shader/ShapeObject3d.PS.hlsl",
			L"ps_6_0"
		);

	Model::GetInstance()->shape_ = shape;

}

void Model::Initialize()
{
}
