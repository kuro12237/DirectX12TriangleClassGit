#include"Cleyera.h"

#include"Model.h"
#include"Line/Line.h"
#include"ObjectManager/ObjectManager.h"

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{

	const int32_t kClientWidth = 1280;
	const int32_t kClientHeight = 720;

	Cleyera::Initialize();

	MSG msg{};

	Transform camera;
	camera.Scale = { 1,1,1 };
	camera.translate = { 0,0,-5 };
	camera.rotate = { 0,0,0 };

	WorldTransform worldTransform_[4];
	worldTransform_[0].Initialize();
	worldTransform_[1].Initialize();
	worldTransform_[2].Initialize();
	worldTransform_[3].Initialize();

	Model* model = new Model;

	Vector4 pos = { 0,0,0,1 };
	float size = 0.5f;

	texResourceProperty texUV =
		TexManager::LoadTexture("Resource/uvChecker.png");
	texResourceProperty texEnemy =
		TexManager::LoadTexture("Resource/Enemy.png");
	texResourceProperty texBlock =
		TexManager::LoadTexture("Resource/block.png");
	texResourceProperty texMonster =
		TexManager::LoadTexture("Resource/monsterBall.png");

	model->Initialize(pos, size, worldTransform_[0], texUV, Sphere);

	Sprite* sprite = new Sprite;
	

	sprite->Initialize({ 0,0 }, 320, worldTransform_[1], texUV);

	bool texFlag = false;

	ObjectManager* obj[2];// = nullptr;


	obj[0] = new ObjectManager;
	obj[0]->Initialize({0,0,0,0}, size, worldTransform_[2], texUV, "Resource", "plane.obj");

	obj[1] = new ObjectManager;
	obj[1]->Initialize({ 0,0,0,0 }, size, worldTransform_[3], texUV, "Resource", "axis.obj");

	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			Cleyera::WinMSG(msg);
		}

		Cleyera::BeginFlame(kClientWidth, kClientHeight);
		ImGui::Begin("sphere");
		ImGui::SliderFloat3("Trans", &worldTransform_[0].translation_.x, -10.0f, 10.0f);
		ImGui::SliderFloat3("rotate", &worldTransform_[0].rotate_.x, -5.0f, 5.0f);
		ImGui::Checkbox("texChange", & texFlag);
		ImGui::End();
		
		if (texFlag)
		{
			model->SetTexProperty(texMonster);
		}
		else
		{
			model->SetTexProperty(texUV);
		}

		Matrix4x4 ShereWorldMatrix = MatrixTransform::MakeAffineMatrix(
			worldTransform_[0].scale_,
			worldTransform_[0].rotate_,
			worldTransform_[0].translation_);

		ShereWorldMatrix = Camera::worldViewProjectionMatrixFanc(ShereWorldMatrix);

		model->TransferMatrix(ShereWorldMatrix);
		//model->Draw();

		ImGui::Begin("plane");

		ImGui::SliderFloat3("trans", &worldTransform_[2].translation_.x, -10.0f, 10.0f);
		ImGui::SliderFloat3("rotate", &worldTransform_[2].rotate_.x, -10.0f, 10.0f);

		ImGui::End();
		ImGui::Begin("axis");

		ImGui::SliderFloat3("trans", &worldTransform_[3].translation_.x, -10.0f, 10.0f);
		ImGui::SliderFloat3("rotate", &worldTransform_[3].rotate_.x, -10.0f, 10.0f);

		ImGui::End();
		Matrix4x4 objMatrix = MatrixTransform::MakeAffineMatrix(
			worldTransform_[2].scale_,
			worldTransform_[2].rotate_,
			worldTransform_[2].translation_);
			objMatrix = Camera::worldViewProjectionMatrixFanc(objMatrix);

			Matrix4x4 obj2Matrix = MatrixTransform::MakeAffineMatrix(
				worldTransform_[3].scale_,
				worldTransform_[3].rotate_,
				worldTransform_[3].translation_);
			obj2Matrix = Camera::worldViewProjectionMatrixFanc(obj2Matrix);

		obj[0]->Draw(objMatrix);
		obj[1]->Draw(obj2Matrix);
		ImGui::Begin("camera");

		ImGui::SliderFloat3("trans", &camera.translate.x, -10.0f, 10.0f);
		ImGui::SliderFloat3("rotate", &camera.rotate.x, -10.0f, 10.0f);

		ImGui::End();

		Camera::SetPosition(camera);

	

		ImGui::Begin("sprite");
		ImGui::SliderFloat3("Trans", &worldTransform_[1].translation_.x, -10.0f, 10.0f);
		ImGui::SliderFloat3("rotate", &worldTransform_[1].rotate_.x, -2.0f, 2.0f);
		ImGui::End();
		Matrix4x4 spriteWorldMatrix = MatrixTransform::MakeAffineMatrix(worldTransform_[1].scale_, worldTransform_[1].rotate_, worldTransform_[1].translation_);
		sprite->TransferMatrix(spriteWorldMatrix);

		sprite->Draw();


		Cleyera::EndFlame();

	}

	texUV = TexManager::Release(texUV);
	texBlock = TexManager::Release(texBlock);
	texEnemy = TexManager::Release(texEnemy);
	texMonster = TexManager::Release(texMonster);

	//mesh->Release();
	model->Release();
	obj[0]->Release();
	obj[1]->Release();
	sprite->Release();
	Cleyera::Finalize();


	return 0;
}
