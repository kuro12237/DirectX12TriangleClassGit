#pragma once
#include"Pch.h"
#include"DebugTools/Commands/ICommand.h"
#include"Grid/Grid.h"


class DebugTools
{
public:
	
	~DebugTools() {};

	static DebugTools* GetInstance();

	/// <summary>
	/// コマンドを追加する
	/// </summary>
	static void addCommand(ICommand* command,const string &log=0);
	
	/// <summary>
    /// 登録したコマンドをすべて消す 
	/// </summary>
	static void ClearCommand();

	/// <summary>
	/// DebugToolを使う場合はSceneのUpdateの最後に必ずはさむ
	/// </summary>
	static ViewProjection ConvertViewProjection(ViewProjection& viewProjection);

	/// <summary>
	/// 登録したコマンドの更新処理
	/// </summary>
	static void UpdateExecute(int commandNumber);

	/// <summary>
	/// 登録したコマンドの描画処理
	/// </summary>
	static void DrawExecute(int commandNumber);

	/// <summary>
	/// gameSceneの内では使わない
	/// </summary>
	static void SetViewProjection(ViewProjection viewProjection) { DebugTools::GetInstance()->viewProjection_ = viewProjection; }

	/// <summary>
	/// gameSceneの内では使わない
	/// </summary>
	static void SetIsviewProjectionReWrite(bool Flag) { DebugTools::GetInstance()->isViewProjectionReWrite = Flag; }

	/// <summary>
	/// gameSceneの内では使わない
	/// </summary>
	static ViewProjection GetViewProjection() { return DebugTools::GetInstance()->viewProjection_; }

private:

	vector<ICommand*> commands;

	ViewProjection viewProjection_{};
	ViewProjection SaveViewProjection_{};
	bool isViewProjectionReWrite=false;
	bool isCameraSave = false;
};
