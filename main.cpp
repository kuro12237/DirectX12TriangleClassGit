#include"CLEYERA/CLEYERA.h"

//ALのworldTransformを実装する

struct Triangle
{
   Vector3 position;
	ResourcePeroperty Resources;
	unsigned int Color;
	Matrix4x4 matrix;
};


int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{


	Cleyera* Cleyera_ = new Cleyera;

	MatrixTransform* matrixTransform = new MatrixTransform();

	WorldTransform worldTransform[2];


	//Size
	const int32_t kClientWidth = 1280;
	const int32_t kClientHeight = 720;


	Cleyera_->Initialize(kClientWidth, kClientHeight);

	//worldTransform[0].Initialize();
	//worldTransform[0].translate_ = { 0.0f,0.5f,0.0f };// , { -0.5f,0.0f,0.0f }, { 0.5f,0.0f,0.0f }



	Triangle triangle[2];
	triangle[0].position =
	{ 0.0f,0.5f,0.0f};

	triangle[0].Resources = Cleyera_->CreateResource();
	triangle[0].Color = 0xff00ffff;
	triangle[0].matrix = matrixTransform->Identity();



	
	triangle[1].position =
	{ 0.0f,1.0f,0.0f };

	triangle[1].Resources = Cleyera_->CreateResource();
	triangle[1].Color = 0xffffffff;
	triangle[1].matrix = matrixTransform->Identity();
	MSG msg{};

	

	while (msg.message!=WM_QUIT)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			Cleyera_->WinMSG(msg);


		}

		Cleyera_->BeginFlame(kClientWidth, kClientHeight);
		

		Cleyera_->TriangleDraw(triangle[0].position,
			triangle[0].Color, triangle[0].matrix,
			triangle[0].Resources);
/*
		Cleyera_->TriangleDraw(triangle[1].position,
			triangle[1].Color, triangle[1].matrix,
			triangle[1].Resources);
			*/
		Cleyera_->EndFlame();
	}

	
	return 0;


}