#include"Cleyera.h"

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	const int32_t kClientWidth = 1280;
	const int32_t kClientHeight = 720;

	Cleyera::Initialize();

	MSG msg{};

	Mesh* mesh[2];
	mesh[0] = new Mesh();
	mesh[1] = new Mesh();

	WorldTransform worldTransform_[3];
	worldTransform_[0].Initialize();
	worldTransform_[1].Initialize();
	worldTransform_[2].Initialize();

	mesh[0]->Initialize(worldTransform_[0], Vector4(0, 0, 0, 1),Vector4(1,1,1,1));
	mesh[1]->Initialize(worldTransform_[1], Vector4(0, 0, 0, 1),Vector4(0,0,1,1));

	Transform CameraTransform = { {1,1,1},{0,0,0},{0,0,-5.0f} };

	texResourceProperty tex;
	tex = TexManager::LoadTexture("Resource/Enemy.png");

	Sprite* sprite[2];
		sprite[0] = new Sprite();
		sprite[1] = new Sprite();
	sprite[0]->Initialize({0,0,0,1}, worldTransform_[2], tex, Triangle);
	sprite[1]->Initialize({ 0,0,0,1 }, worldTransform_[2], tex, Box);

	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			Cleyera::WinMSG(msg);
		}

		Cleyera::BeginFlame(kClientWidth, kClientHeight);
		


		//«shape
		worldTransform_[0].matWorld = MatrixTransform::MakeAffineMatrix(worldTransform_[0].scale_, worldTransform_[0].rotate_, worldTransform_[0].translation_);
		
		worldTransform_[0].matWorld = Camera::worldViewProjectionMatrixFanc(worldTransform_[0].matWorld);

		mesh[0]->TransferMatrix(worldTransform_[0].matWorld);

		worldTransform_[1].matWorld = MatrixTransform::MakeAffineMatrix(worldTransform_[1].scale_, worldTransform_[1].rotate_, worldTransform_[1].translation_);

		worldTransform_[1].matWorld = Camera::worldViewProjectionMatrixFanc(worldTransform_[1].matWorld);

		mesh[1]->TransferMatrix(worldTransform_[1].matWorld);

		///«Sprite
		worldTransform_[2].matWorld = MatrixTransform::MakeAffineMatrix(worldTransform_[2].scale_, worldTransform_[2].rotate_, worldTransform_[2].translation_);

		worldTransform_[2].matWorld =
			Camera::worldOthographicMatrix(worldTransform_[2].matWorld);
			//Camera::worldViewProjectionMatrixFanc(worldTransform_[2].matWorld);

		sprite[1]->TransferMatrix(worldTransform_[2].matWorld);



		for (int i = 0; i < 2; i++)
		{
			mesh[i]->Draw();
		}
		//sprite[0]->Draw();
		sprite[1]->Draw();
	
		ImGui::Begin("Mesh1");
		ImGui::SliderFloat3("MeshTrans_1", &worldTransform_[0].translation_.x, -1.0f, 1.0f);
		ImGui::SliderFloat3("MeshRotate_1", &worldTransform_[0].rotate_.x, -1.0f, 1.0f);
		ImGui::End();
		
		ImGui::Begin("Mesh_2");
		ImGui::SliderFloat3("MeshTrans_2", &worldTransform_[1].translation_.x, -1.0f, 1.0f);
		ImGui::SliderFloat3("MeshRotate_2", &worldTransform_[1].rotate_.x, -1.0f, 1.0f);
		ImGui::End();

		worldTransform_[2].matWorld = sprite[1]->GetWorldTransform();

		ImGui::Begin("SpriteBOX");
		ImGui::Text("x::%f  y::%f  z::%f", 
			worldTransform_[2].matWorld.m[3][0],
			worldTransform_[2].matWorld.m[3][1],
			worldTransform_[2].matWorld.m[3][2]);
		ImGui::SliderFloat3("spriteTrans", &worldTransform_[2].translation_.x, -1.0f, 1.0f);
		ImGui::End();

		ImGui::Begin("Camera");
		ImGui::SliderFloat3("Camera", &CameraTransform.translate.x, -100.0f, 100.0f);
		ImGui::End();
		Camera::SetPosition(CameraTransform);

		Cleyera::EndFlame();

	}

	mesh[0]->Release();
	mesh[1]->Release();
	

	tex = TexManager::Release(tex);
	sprite[0]->Release();
	sprite[1]->Release();


	Cleyera::Finalize();


	return 0;
}
