#pragma once
#include"MatrixTransform.h"
#include"VectorTransform.h"

struct SCameraPeloperty
{
	Transform Transform;
	Matrix4x4 Matrix;
	Matrix4x4 ProjectionMatrix;
	Matrix4x4 OrthographicMatrix;
};

class Camera
{
public:
	Camera();
	~Camera();

	static Camera* GetInstance();

	static void Initialize(const int32_t  kClientWidth, const int32_t  kClientHeight);

	static void Finalize();

	static void SetPosition(Transform Transform);

	static void Update(Transform Transform);

	static Matrix4x4 worldViewProjectionMatrixFanc(Matrix4x4 matrix);



private:

	
	float aspectRatio;
	SCameraPeloperty Camera_;

	MatrixTransform* matrixTransform=nullptr;
	VectorTransform* vectorTransform=nullptr;
};
