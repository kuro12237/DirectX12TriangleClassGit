#pragma once
#include"Pch.h"
#include"DebugTools/Commands/ICommand.h"
#include"Grid/Grid.h"


class DebugTools
{
public:
	
	~DebugTools() {};

	static DebugTools* GetInstance();

	static void addCommand(ICommand* command);

	static void SetViewProjection(ViewProjection &viewProjection) { DebugTools::GetInstance()->viewProjection_ = viewProjection; }

	static void Execute(int buttonIndex);

private:


	vector<ICommand*> commands;

	ViewProjection viewProjection_{};
};
