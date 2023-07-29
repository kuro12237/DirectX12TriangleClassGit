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


	texHandule = Cleyera::LoadTex("CLEYERA/DefaultResources/uvChecker.png");


}

void EngineDebug::Update(Game* game)
{
	game;

	ImGui::Begin("ShapeModel");
	ImGui::SliderFloat3("leftPos", &triangle[0].position.left.x, -5.0f, 5.0f);
	ImGui::SliderFloat3("TopPos", &triangle[0].position.top.x, -5.0f, 5.0f);
	ImGui::SliderFloat3("RightPos", &triangle[0].position.right.x, -5.0f, 5.0f);
	ImGui::SliderFloat3("Color", &triangle[0].Color.x, 0.0f, 1.0f);
	ImGui::End();

	ImGui::Begin("TexModel");
	ImGui::SliderFloat3("leftPos", &triangle[1].position.left.x, -5.0f, 5.0f);
	ImGui::SliderFloat3("TopPos", &triangle[1].position.top.x, -5.0f, 5.0f);
	ImGui::SliderFloat3("RightPos", &triangle[1].position.right.x, -5.0f, 5.0f);
	ImGui::End();


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
