#include "WorldTransform.h"

void WorldTransform::Initialize()
{
	matWorld = MatrixTransform::Identity();
	
}

void WorldTransform::UpdateMatrix()
{
	matWorld = MatrixTransform::AffineMatrix(scale, rotation, translate);
}

void WorldTransform::TransfarMatrix(ComPtr<ID3D12Resource>& wvpResource, ViewProjection *viewProjection)
{
	TransformationMatrix* wvp = {};
	wvpResource->Map(0, nullptr, reinterpret_cast<void**>(&wvp));

	matWorld = MatrixTransform::Multiply(matWorld, MatrixTransform::Multiply(viewProjection->GetViewMatrix(),viewProjection->GetMatProjection()));

	wvp->WVP = matWorld;
	wvp->world = matWorld;
	
}
