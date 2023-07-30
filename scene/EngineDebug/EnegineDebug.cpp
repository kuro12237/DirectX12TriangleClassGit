#include"EngineDebug.h"



void EngineDebug::Initialize(Game* game)
{
	game;
	matrixTransform_ = new MatrixTransform();

	mode = Shape;

	//t,l,r
	triangle[0].position =
	{ {0.0f,0.0,0.0f},{-0.5f,-1.0f,0.0f},{0.5f,-1.0f,0.0f } };
	triangle[0].Resources = Cleyera::CreateShapeResource();
	triangle[0].Color={0.0f,0.0f,0.0f,1.0f};
	triangle[0].matrix = matrixTransform_->Identity();

	triangle[1].position =
	{
	{ 0.5f, 1.0f, 0.0f},
	{ 0.0f,0.0f,0.0f },
	{ 1.0f,-0.0f,0.0f} };
	triangle[1].Resources = Cleyera::CreateSpriteResource();
	triangle[1].Color = {0.0f,0.0f,0.0f,0.0f};
	
	triangle[1].matrix = matrixTransform_->Identity();


	texHandule = Cleyera::LoadTex("Resource/uvChecker.png");

	for (int i = 0; i < TriangleMax; i++)
	{
		scale_[i] = { 1.0f,1.0f,1.0f };
		rotate_[i] = { 0.0f,0.0f,0.0f };
		translate_[i] = {0.0f,0.0f,0.0f};
	}
}

void EngineDebug::Update(Game* game)
{
	game;

	ImGui::Begin("ShapeModel");
	ImGui::Text("pos");
	ImGui::SliderFloat3("leftPos", &triangle[0].position.left.x, -5.0f, 5.0f);
	ImGui::SliderFloat3("TopPos", &triangle[0].position.top.x, -5.0f, 5.0f);
	ImGui::SliderFloat3("RightPos", &triangle[0].position.right.x, -5.0f, 5.0f);
	ImGui::ColorEdit3("Color", (float*)&triangle[0].Color);
	ImGui::Text("WorldTransform");
	ImGui::SliderFloat3("Translate", &translate_[0].x, -5.0f, 5.0f);
	ImGui::SliderFloat3("Rotate", &rotate_[0].x, -5.0f, 5.0f);
	ImGui::SliderFloat3("Scale", &scale_[0].x, -5.0f, 5.0f);
	ImGui::End();




	ImGui::Begin("TexModel");
	ImGui::SliderFloat3("leftPos", &triangle[1].position.left.x, -5.0f, 5.0f);
	ImGui::SliderFloat3("TopPos", &triangle[1].position.top.x, -5.0f, 5.0f);
	ImGui::SliderFloat3("RightPos", &triangle[1].position.right.x, -5.0f, 5.0f);
	ImGui::Text("WorldTransform");
	ImGui::SliderFloat3("Translate", &translate_[1].x, -5.0f, 5.0f);
	ImGui::SliderFloat3("Rotate", &rotate_[1].x, -5.0f, 5.0f);
	ImGui::SliderFloat3("Scale", &scale_[1].x, -5.0f, 5.0f);

	ImGui::End();

	for (int i = 0; i < TriangleMax; i++)
	{

		triangle[i].matrix = matrixTransform_->MakeAffineMatrix(scale_[i], rotate_[i], translate_[i]);

	}


}

void EngineDebug::Draw(Game* game)
{
	game;
	


		Cleyera::SpriteTriangleDraw(
			triangle[1].position,
			WHITE,
			triangle[1].matrix,
			triangle[1].Resources,
			texHandule);

		Cleyera::TriangleDraw(triangle[0].position,
			triangle[0].Color, triangle[0].matrix,
			triangle[0].Resources);





}

void EngineDebug::Release(Game* game)
{
	game;
	Cleyera::TriangleResourceRelease(triangle[0].Resources);
	Cleyera::SpriteTriangleResourceRelease(triangle[1].Resources, texHandule);

}
