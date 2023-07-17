#include"EngineDebug.h"



void EngineDebug::Initialize(Game* game)
{
	game;
	matrixTransform_ = new MatrixTransform();

	//t,l,r
	triangle[0].position =
	{ {0.0f,0.0,0.0f},{-0.5f,-1.0f,0.0f},{0.5f,-1.0f,0.0f } };
	triangle[0].Resources = Cleyera::CreateShapeResource();
	triangle[0].Color = BLACK;
	triangle[0].matrix = matrixTransform_->Identity();

	triangle[1].position =
	{
	{ 0.0f, 1.0f, 0.0f},
	{ -0.5f,0.0f,0.0f },
	{ 0.5f,-0.0f,0.0f} };
	triangle[1].Resources = Cleyera::CreateSpriteResource();
	triangle[1].Color = WHITE;
	triangle[1].matrix = matrixTransform_->Identity();



	

	texHandule = Cleyera::LoadTex("CLEYERA/DefaultResources/uvChecker.png");


	triangle[2].position =
	{
	{ -0.5f, 1.0f, 0.0f},
	{ -1.0f,0.0f,0.0f },
	{ 0.0f,0.0f,0.0f} };
	triangle[2].Resources = Cleyera::CreateSpriteResource();
	triangle[2].Color = WHITE;
	triangle[2].matrix = matrixTransform_->Identity();
}

void EngineDebug::Update(Game* game)
{
	game;

	StateTimer--;
	if (StateTimer<0)
	{
		//Game::ChangeState(new Title());

	}
	
}

void EngineDebug::Draw(Game* game)
{
	game;

	Cleyera::TriangleDraw(triangle[0].position,
		triangle[0].Color, triangle[0].matrix,
		triangle[0].Resources);
	Cleyera::SpriteTriangleDraw(
		triangle[1].position,
		triangle[1].Color,
		triangle[1].matrix,
		triangle[1].Resources,
		texHandule);

	Cleyera::SpriteTriangleDraw(
		triangle[2].position,
		triangle[2].Color,
		triangle[2].matrix,
		triangle[2].Resources,
		texHandule);

}

void EngineDebug::Release(Game* game)
{
	game;
	Cleyera::TriangleResourceRelease(triangle[0].Resources);
	Cleyera::SpriteTriangleResourceRelease(triangle[1].Resources, texHandule);
	Cleyera::SpriteTriangleResourceRelease(triangle[2].Resources, texHandule);

}
