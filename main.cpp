#include"Cleyera.h"
#define TRIANGLE_MAX 2


int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{

	///初期化
	//ウインドウサイズの設定
	const int32_t kClientWidth = 1280;
	const int32_t kClientHeight = 720;

	Cleyera::Initialize(kClientWidth, kClientHeight);
	
	
	Model* model[TRIANGLE_MAX];

	Vector3 pos_[TRIANGLE_MAX];
	WorldTransform worldTransform_[TRIANGLE_MAX];

	pos_[0] = {-0.5f,0.0f,0};
	pos_[1] = { 0.5f,0.0f,0};


	//model
	for (int i = 0; i < TRIANGLE_MAX; i++)
	{
		model[i] = new Model();

		model[i]->Initialize();
	}



	for (int i = 0; i < TRIANGLE_MAX; i++)
	{
		
		model[i]->SetPos(pos_[i]);
		model[i]->SetSize(0.5f);
	}
	
	MSG msg{};

	//ゲーム更新処理
	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			Cleyera::WinMSG(msg);
		}

		Cleyera::BeginFlame(kClientWidth,kClientHeight);


		///ゲームシーン
		//更新
		
	
		
		//更新終了

		//描画

		
		for (int i = 0; i < TRIANGLE_MAX; i++)
		{
			model[i]->Draw();
		}
		//描画終了

		Cleyera::EndFlame();
	}

	//頂点などのの解放処理
	for (int i = 0; i < TRIANGLE_MAX; i++)
	{
		model[i]->ResourceRelease();
	}

	Cleyera::Finalize();
	return 0;
}
