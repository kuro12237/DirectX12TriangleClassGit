#include"Cleyera.h"


int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{

	///初期化
	//ウインドウサイズの設定
	const int32_t kClientWidth = 1280;
	const int32_t kClientHeight = 720;

	Cleyera::Initialize(kClientWidth, kClientHeight);
	
	MatrixTransform *matrixTransform_ = new MatrixTransform();


	Vector3 pos_ = { 0,0,0 };
	ResourcePeroperty resource_=Cleyera::CreateResource();
	unsigned int color_=BLACK;
	WorldTransform worldTransform_;
	
	worldTransform_.matWorld_=matrixTransform_->Identity();
	float size = 1;

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
		
	
		
		worldTransform_.UpdateMatrix();
		//更新終了

		//描画
		Cleyera::TriangleDraw(
			pos_,
			size,
			color_,
			worldTransform_, 
			resource_);
		
		//描画終了

		Cleyera::EndFlame();
	}

	//頂点などのゲームシーンの解放処理
	Cleyera::ResourceRelease(resource_);


	Cleyera::Finalize();
	return 0;
}
