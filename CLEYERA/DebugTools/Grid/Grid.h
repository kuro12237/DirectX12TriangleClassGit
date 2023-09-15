#pragma once
#include"DebugTools/Commands/ICommand.h"
#include"Model.h"
#define XYGridLine_Max 20


class Grid :public ICommand
{
public:
	~Grid() {};

	static Grid* GetInstance();

	static void Initialize();

	void execute(ViewProjection viewProjection)override;
private:

	unique_ptr<Model> XLine_[XYGridLine_Max] = {};
	WorldTransform XworldTransform_[XYGridLine_Max] = {};

	unique_ptr<Model>ZLine_[XYGridLine_Max] = {};
	WorldTransform ZworldTransform_[XYGridLine_Max] = {};

	unique_ptr <Model>YLine_ = {};
	WorldTransform YworldTransform_ = {};
};

