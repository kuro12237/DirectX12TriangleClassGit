#pragma once
#include"Setup/WinSetup.h"
#include<string>
#include<format>
#include "Setup/DirectXSetup.h"

class Adapter
{
public:
	Adapter();
	~Adapter();

	/// <summary>
	/// Adapterの初期化
	/// </summary>
	/// <param name="Width"></param>
	/// <param name="Height"></param>
	void Initialize(const int32_t Width, const int32_t Height);

	/// <summary>
	/// WinMessageをWinSetupに送る
	/// </summary>
	/// <param name="msg"></param>
	void WinMSG( MSG &msg);

	/// <summary>
	/// ループの最初に行う処理
	/// </summary>
	/// <param name="kClientWidth"></param>
	/// <param name="kClientHeight"></param>
	void BeginFlame(const int32_t kClientWidth, const int32_t kClientHeight);

	/// <summary>
	/// ループの最終処理
	/// </summary>
	void EndFlame();

	/// <summary>
	/// 三角形の頂点の生成
	/// </summary>
	/// <param name="vertex"></param>
	void VertexCreate(VertexProperty &vertex);
		 
	/// <summary>
	/// 三角形の表示
	/// </summary>
	/// <param name="top"></param>
	/// <param name="left"></param>
	/// <param name="right"></param>
	/// <param name="三角形の頂点"></param>
	void TriangleDraw(Vec4 top, Vec4 left, Vec4 right, VertexProperty vertex);

	/// <summary>
	/// adapterのデリーと::絶対に先に三角形の頂点をReleaseすること
	/// </summary>
	void Deleate();

private:


	WindowsSetup* WinSetup_ = new WindowsSetup();
	DirectXSetup* DXSetup_ = new DirectXSetup();

	
};



