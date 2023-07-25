#pragma once
#include<format>

#include"Model.h"
#include"ImGuiManager.h"
#include"Camera.h"
#include"WinApp.h"
#include"DirectXSetup.h"


class Cleyera
{
public:
	Cleyera();
	~Cleyera();

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="kClientWidth"></param>
	/// <param name="kClientHeight"></param>
	static void Initialize(const int32_t  kClientWidth, const int32_t  kClientHeight);

	/// <summary>
	/// 解放
	/// </summary>
	static void Finalize();
#pragma region ループする必要がある処理


	/// <summary>
	/// メッセージをWinSetupに転送
	/// </summary>
	/// <param name="msg"></param>
	static void WinMSG(MSG msg);

	static void BeginFlame(const int32_t  kClientWidth, const int32_t  kClientHeight);

	static void EndFlame();
		  

#pragma region 三角形

	/// <summary>
	/// 頂点を作る
	/// </summary>
	/// <param name="Resource"></param>
	static	ResourcePeroperty CreateResource();

	/// <summary>
	/// 三角形のResourceの解放
	/// </summary>
	static void ResourceRelease(
		ResourcePeroperty Resource);
	
	/// <summary>
	/// 三角形の表示
	/// </summary>
	/// <param name="position"></param>
	/// <param name="Color"></param>
	/// <param name="worldTransform"></param>
	/// <param name="Resource"></param>
	static void TriangleDraw(
        Vector3 position,
		int size,
		unsigned int Color,
		WorldTransform worldTransform,
		ResourcePeroperty Resource);

private:
	

};

