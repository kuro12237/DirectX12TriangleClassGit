#pragma once
#include<format>
#include"Setup/Win/WinSetup.h"
#include"Setup/DX/DirectXSetup.h"
#include"Models/Model.h"
class Cleyera
{
public:
	Cleyera();
	~Cleyera();


	void Initialize(const int32_t  kClientWidth, const int32_t  kClientHeight);

#pragma region ループする必要がある処理


	/// <summary>
	/// メッセージをWinSetupに転送
	/// </summary>
	/// <param name="msg"></param>
	void WinMSG(MSG msg);


	void BeginFlame(const int32_t  kClientWidth, const int32_t  kClientHeight);

	void EndFlame();
		  
#pragma region 三角形

	/// <summary>
	/// 頂点を作る
	/// </summary>
	/// <param name="Resource"></param>
	ResourcePeroperty CreateResource();

	void TriangleDraw(Position position,unsigned int Color, Matrix4x4 worldTransform, ResourcePeroperty Resource);

#pragma endregion


#pragma endregion


private:


	WindowsSetup* WinSetup = nullptr;
	DirectXSetup* DXSetup = nullptr;
	Model* model = nullptr;

};

