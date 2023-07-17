#include"CLEYERA/CLEYERA.h"



struct Triangle
{
	Position position;
	ResourcePeroperty Resources;
	unsigned int Color;
	Matrix4x4 matrix;
};


int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{


	Cleyera* Cleyera_ = new Cleyera;

	MatrixTransform* matrixTransform = new MatrixTransform();

	//Size
	const int32_t kClientWidth = 1280;
	const int32_t kClientHeight = 720;
	
	
	Cleyera_->Initialize(kClientWidth,kClientHeight);

	Triangle triangle[2];
	triangle[0].position =
	{ {0.0f,0.5,0.0f},{-0.5f,0.0f,0.0f},{0.5f,0.0f,0.0f } };
	triangle[0].Resources = Cleyera_->CreateResource();
	triangle[0].Color = 0xff0000ff;
	triangle[0].matrix = matrixTransform->Identity();



	
	triangle[1].position =
	{ {0.0f,1.0,0.0f},{-0.5f,0.5f,0.0f},{0.5f,0.5f,0.0f } };
	triangle[1].Resources = Cleyera_->CreateResource();
	triangle[1].Color = 0xff0000ff;
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

		Cleyera_->TriangleDraw(triangle[1].position,
			triangle[1].Color, triangle[1].matrix,
			triangle[1].Resources);

		Cleyera_->EndFlame();
	}

	
	return 0;


}