#include "Grid.h"

Grid* Grid::GetInstance()
{
	static Grid instance;
	return &instance;
}

void Grid::Initialize()
{
	for (int i = 0; i < XYGridLine_Max; i++)
	{
		Grid::GetInstance()->XLine_[i] = make_unique<Model>();
		Grid::GetInstance()->XworldTransform_[i].Initialize();

		Grid::GetInstance()->ZLine_[i] = make_unique<Model>();
		Grid::GetInstance()->ZworldTransform_[i].Initialize();



		Grid::GetInstance()->ZLine_[i]->Initialize(new ModelLineState, { float(i-(XYGridLine_Max)/2),0,-10,1 }, { float(i - (XYGridLine_Max)/2),0,10,1 }, { 0,1,0,1 });
		Grid::GetInstance()->XLine_[i]->Initialize(new ModelLineState, { -10,0,float(i - (XYGridLine_Max) / 2),1 }, { 10 ,0,float(i - (XYGridLine_Max) / 2),1 }, { 1,0,0,1 });
	}
	Grid::GetInstance()->YLine_ = make_unique<Model>();
	Grid::GetInstance()->YLine_->Initialize(new ModelLineState, { 0,-10,0,1 }, { 0,10,0,1 }, { 0,0,1,1 });
	Grid::GetInstance()->YworldTransform_.Initialize();
}

void Grid::execute(ViewProjection viewProjection)
{
	for (int i = 0; i < XYGridLine_Max; i++)
	{
		XworldTransform_[i].UpdateMatrix();
		Grid::GetInstance()->XLine_[i]->Draw(XworldTransform_[i], viewProjection);
		
		ZworldTransform_[i].UpdateMatrix();
		Grid::GetInstance()->ZLine_[i]->Draw(ZworldTransform_[i], viewProjection);

	}

	YworldTransform_.UpdateMatrix();

	Grid::GetInstance()->YLine_->Draw(YworldTransform_, viewProjection);

}
