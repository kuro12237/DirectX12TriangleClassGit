#include"WorldTransform.h"

void WorldTransform::Initiallize()
{
	
	matWorld_ =matrixTransform->Identity();

}

void WorldTransform::UpdateMatrix()
{
	
	matWorld_ = matrixTransform->MakeAffineMatrix(scale_, rotate_, translate_);

}
