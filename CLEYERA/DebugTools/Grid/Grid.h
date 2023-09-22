#pragma once
#include"DebugTools/Commands/ICommand.h"
#include"Model.h"
#define XYGridLine_Max 50
#include"VectorTransform.h"

class Grid :public ICommand
{
public:
	~Grid() {};

	static Grid* GetInstance();

	void Initialize();

	void UpdateExecute()override;
	void DrawExecute(ViewProjection viewProjection)override;
private:

	unique_ptr<Model> XLine_[XYGridLine_Max] = {};
	WorldTransform XworldTransform_[XYGridLine_Max] = {};

	unique_ptr<Model>ZLine_[XYGridLine_Max] = {};
	WorldTransform ZworldTransform_[XYGridLine_Max] = {};

	unique_ptr <Model>YLine_ = {};
	WorldTransform YworldTransform_ = {};

	unique_ptr <Model>CenterPoint_ = {};
	WorldTransform CenterWorldTransform_ = {};

	
};

