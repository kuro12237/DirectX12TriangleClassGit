#include"Cleyera.h"
#include"MatrixTransform.h"

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	const int32_t kClientWidth = 1280;
	const int32_t kClientHeight = 720;

	Cleyera::Initialize(kClientWidth, kClientHeight);

	MSG msg{};

	Mesh* mesh[2];
	mesh[0] = new Mesh();
	mesh[1] = new Mesh();

	WorldTransform worldTransform_[2];
	worldTransform_[0].Initialize();
	worldTransform_[1].Initialize();

	
	mesh[0]->Initialize(worldTransform_[0], Vector4(0, 0, 0, 1));
	mesh[1]->Initialize(worldTransform_[1],Vector4(0, 0, 0, 1));

	Transform transform = { {1,1,1},{0,0,0},{0,0,-5.0f} };


	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			Cleyera::WinMSG(msg);
		}

		Cleyera::BeginFlame(kClientWidth, kClientHeight);
		
		ImGui::Begin("Mesh1");
		ImGui::SliderFloat3("trans", &worldTransform_[0].translation_.x, -1.0f, 1.0f);
		ImGui::SliderFloat3("Cameratrans", &transform.translate.x, -5.0f, 5.0f);
		ImGui::End();

		Camera::SetPosition(transform);

		
		worldTransform_[0].matWorld = MatrixTransform::MakeAffineMatrix(worldTransform_[0].scale_, worldTransform_[0].rotate_, worldTransform_[0].translation_);
		
		worldTransform_[0].matWorld = Camera::worldViewProjectionMatrixFanc(worldTransform_[0].matWorld);

		mesh[0]->TransferMatrix(worldTransform_[0].matWorld);
		
		for (int i = 0; i < 1; i++)
		{
			mesh[i]->Draw();
		}
		
	
		Cleyera::EndFlame();

	}

	mesh[0]->Release();
	mesh[1]->Release();

	Cleyera::Finalize();


	return 0;
}
