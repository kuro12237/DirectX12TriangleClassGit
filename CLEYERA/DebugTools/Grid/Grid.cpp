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

		Grid::GetInstance()->ZLine_[i]->Initialize(new ModelLineState,
			{ float(i-(XYGridLine_Max)/2),0.01f,-float(XYGridLine_Max)/2,1 },
			{ float(i - (XYGridLine_Max)/2),0.01f,float(XYGridLine_Max)/2,1 },
			{ 0,1,0,0.5f }
		);
		Grid::GetInstance()->XLine_[i]->Initialize(new ModelLineState,
			{ -float(XYGridLine_Max)/2,0.01f,float(i - (XYGridLine_Max) / 2),1 },
			{ float(XYGridLine_Max)/2 ,0.01f,float(i - (XYGridLine_Max) / 2),1 },
			{ 1,0,0,0.5f });
	}

	Grid::GetInstance()->YLine_ = make_unique<Model>();
	Grid::GetInstance()->YLine_->Initialize(new ModelLineState, { 0,-float(XYGridLine_Max),0,1 }, { 0,float(XYGridLine_Max),0,1 }, { 0,0,1,0.5f });
	Grid::GetInstance()->YworldTransform_.Initialize();

	Grid::GetInstance()->CenterPoint_ = make_unique<Model>();

	Vector4 color = Model::ColorConversion(0x31383fff);
	Grid::GetInstance()->CenterPoint_->Initialize(new ModelPlaneState,{0,-0.0f,0.0f,1},float(XYGridLine_Max/2),color);
	
	Grid::GetInstance()->CenterWorldTransform_.Initialize();

}

void Grid::UpdateExecute()
{
	for (int i = 0; i < XYGridLine_Max; i++)
	{
		XworldTransform_[i].UpdateMatrix();
		ZworldTransform_[i].UpdateMatrix();
	}
	YworldTransform_.UpdateMatrix();
	CenterWorldTransform_.UpdateMatrix();
}

void Grid::DrawExecute(ViewProjection viewProjection)
{
	for (int i = 0; i < XYGridLine_Max; i++)
	{
		Grid::GetInstance()->XLine_[i]->Draw(XworldTransform_[i], viewProjection);
		Grid::GetInstance()->ZLine_[i]->Draw(ZworldTransform_[i], viewProjection);
	}

	Grid::GetInstance()->YLine_->Draw(YworldTransform_, viewProjection);
	Grid::GetInstance()->CenterPoint_->Draw(CenterWorldTransform_, viewProjection);
}
