#include "ViewProjection.h"

void ViewProjection::Initialize(Vector3 r, Vector3 t)
{
	rotation_= r;
	translation_ = t;
}

void ViewProjection::UpdateMatrix()
{
	Matrix4x4 translateMatrix = MatrixTransform::TranslateMatrix(translation_);
	Matrix4x4 rotateXMatrix = MatrixTransform::RotateXMatrix(rotation_.x);
	Matrix4x4 rotateYMatrix = MatrixTransform::RotateYMatrix(rotation_.y);
	Matrix4x4 rotateZMatrix = MatrixTransform::RotateZMatrix(rotation_.z);
	Matrix4x4 rotateMatrix = MatrixTransform::Multiply(rotateXMatrix, MatrixTransform::Multiply(rotateYMatrix, rotateZMatrix));
	

	matView_ = MatrixTransform::Multiply(MatrixTransform::Inverse(translateMatrix), MatrixTransform::Inverse(rotateMatrix));
	matProjection_ = MatrixTransform::PerspectiveFovMatrix(fov_, aspectRatio_, nearClip_, farClip_);
}
