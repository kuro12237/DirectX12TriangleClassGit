#pragma once
#include"Matrix2x2.h"
#include"Matrix3x3.h"
#include"Matrix4x4.h"
#include"../Vector/Vector4.h"
#include"../Vector/Vector3.h"

#include"../Vector/Vector2.h"

#include<cmath>


#include <cassert>

struct Transform
{
	Vector3 Scale;
	Vector3 rotate;
	Vector3 translate;
};
class MatrixTransform
{
public:
	MatrixTransform();
	~MatrixTransform();

#pragma region �O�����s��
#pragma region +-*

	/// <summary>
	/// 3x3�s��̑����Z
	/// </summary>
	/// <param name="m1"></param>
	/// <param name="m2"></param>
	/// <returns></returns>
	Matrix3x3 Add(const Matrix3x3& m1, const Matrix3x3& m2);

	/// <summary>
	/// 3x3�s��̈����Z
	/// </summary>
	/// <param name="m1"></param>
	/// <param name="m2"></param>
	/// <returns></returns>
	Matrix3x3 Subract(const Matrix3x3& m1, const Matrix3x3& m2);


	/// <summary>
	/// 3x3�s��̊|���Z
	/// </summary>
	/// <param name="m1"></param>
	/// <param name="m2"></param>
	/// <returns></returns>
	Matrix3x3 Multiply(const Matrix3x3& m1, const Matrix3x3& m2);


#pragma endregion 

#pragma region �ړ��E��]�E�傫��



	/// <summary>
	/// 3x3�s��̕��s�ړ�
	/// </summary>
	/// <param name="translate"></param>
	/// <returns></returns>
	Matrix3x3 MakeTranslate3x3Matrix(Vector2 translate);

	/// <summary>
	/// 3x3�s���]
	/// </summary>
	/// <param name="theta"></param>
	/// <returns></returns>
	Matrix3x3 MakeRotate3x3Matrix(float radian);

	/// <summary>
	/// 3x3�s��̑傫��
	/// </summary>
	/// <param name="scale"></param>
	/// <returns></returns>
	Matrix3x3 MakeScaleMatrix(const Vector2 scale);



#pragma endregion

#pragma region �s��̕ϊ�

	/// <summary>
	/// 3x3�̃A�t�B���ϊ�
	/// </summary>
	/// <param name="translate"></param>
	/// <param name="radian"></param>
	/// <param name="scale"></param>
	/// <returns></returns>
	Matrix3x3 MakeAffineMatrix(const Vector2 translate, float radian, const Vector2 scale);


	/// <summary>
	/// 2�����x�N�g����3x3�s��ŕϊ�
	/// </summary>
	/// <param name="v"></param>
	/// <param name="matrix"></param>
	/// <returns></returns>
	Vector2 Transform(Vector2 v, Matrix3x3 matrix);

	/// <summary>
	/// 3x3�̋t�s��
	/// </summary>
	/// <param name="matrix"></param>
	/// <returns></returns>
	Matrix3x3 Inverse(Matrix3x3 matrix);

	/// <summary>
	/// 3x3�̓]�u�s��
	/// </summary>
	/// <param name="matrix"></param>
	/// <returns></returns>
	Matrix3x3 Transpose(Matrix3x3 matrix);

	/// <summary>
	/// 3x3�̐��ˉe�s��
	/// </summary>
	/// <param name="left"></param>
	/// <param name="top"></param>
	/// <param name="right"></param>
	/// <param name="bottom"></param>
	/// <returns></returns>
	Matrix3x3 MakeOrthographicMatrix(float left, float top, float right, float bottom);

	/// <summary>
	/// 3x3�r���[�|�[�g�s��
	/// </summary>
	/// <param name="left"></param>
	/// <param name="top"></param>
	/// <param name="width"></param>
	/// <param name="height"></param>
	/// <returns></returns>
	Matrix3x3 MakeviewportMatrix(float left, float top, float width, float height);
#pragma endregion


#pragma endregion

#pragma region 4����

#pragma region +-*

	/// <summary>
	/// 4x4�s��̑����Z
	/// </summary>
	/// <param name="m1"></param>
	/// <param name="m2"></param>
	/// <returns></returns>
	Matrix4x4 Add(const Matrix4x4& m1, const Matrix4x4& m2);

	/// <summary>
	/// 4x4�s��̈����Z
	/// </summary>
	/// <param name="m1"></param>
	/// <param name="m2"></param>
	/// <returns></returns>
	Matrix4x4 Subract(const Matrix4x4& m1, const Matrix4x4& m2);


	/// <summary>
	/// 4x4�s��̏�Z
	/// </summary>
	/// <param name="m1"></param>
	/// <param name="m2"></param>
	/// <returns></returns>
	Matrix4x4 Multiply(Matrix4x4 m1, Matrix4x4 m2);
#pragma endregion 

#pragma region �s��̈ړ��E�傫���E��]
	/// <summary>
	/// 4x4�X�P�[���s��
	/// </summary>
	/// <param name="scale"></param>
	/// <returns></returns>
	Matrix4x4 MakeScaleMatrix(const Vector3 scale);

	/// <summary>
	/// 4x4���s�ړ��s��
	/// </summary>
	/// <param name="translate"></param>
	/// <returns></returns>
	Matrix4x4 MakeTranslateMatrix(Vector3 translate);

#pragma region ��]
	/// <summary>
	/// 4x4Y���̉�]
	/// </summary>
	/// <param name="�p�x"></param>
	/// <returns></returns>
	Matrix4x4 MakeRotateXMatrix(float radian);

	/// <summary>
	/// 4x4X���̉�]
	/// </summary>
	/// <param name="�p�x"></param>
	/// <returns></returns>
	Matrix4x4 MakeRotateYMatrix(float radian);

	/// <summary>
	/// 4x4Z���̉�]
	/// </summary>
	/// <param name="�p�x"></param>
	/// <returns></returns>
	Matrix4x4 MakeRotateZMatrix(float radian);


	/// <summary>
	/// XYZ���ׂẲ�]
	/// </summary>
	/// <param name="Xradian"></param>
	/// <param name="Yradian"></param>
	/// <param name="Zradian"></param>
	/// <returns></returns>
	Matrix4x4 MakeRotateXYZMatrix(float Xradian, float radianY, float Zradian);
#pragma endregion

#pragma endregion

#pragma region �s��̕ϊ�

	/// <summary>
	/// 4x4�A�t�B���ϊ��s��
	/// </summary>
	/// <param name="scale"></param>
	/// <param name="rotate"></param>
	/// <param name="translate"></param>
	/// <returns></returns>
	Matrix4x4 MakeAffineMatrix(const Vector3& scale, const Vector3& rotate, const Vector3& translate);

	/// <summary>
	/// 4x4�]���s��
	/// </summary>
	/// <param name="m"></param>
	/// <returns></returns>
	Matrix4x4 Transpose(const Matrix4x4 m);

	/// <summary>
	/// 4x4�t�s��
	/// </summary>
	/// <param name="matrix"></param>
	/// <returns></returns>
	Matrix4x4 Inverse(Matrix4x4& matrix);

	/// <summary>
	/// 4x4�P�ʍs��
	/// </summary>
	/// <returns></returns>
	Matrix4x4 Identity();


	/// <summary>
	/// 4x4���ˉe�s��
	/// </summary>
	/// <param name="left"></param>
	/// <param name="top"></param>
	/// <param name="right"></param>
	/// <param name="bottom"></param>
	/// <param name="neaCrlip"></param>
	/// <param name="farClip"></param>
	/// <returns></returns>
	Matrix4x4 MakeOrthographicMatrix(float left, float top, float right, float bottom, float neaCrlip, float farClip);

	/// <summary>
	/// 4x4�r���[�|�[�g�s��
	/// </summary>
	/// <param name="left"></param>
	/// <param name="top"></param>
	/// <param name="width"></param>
	/// <param name="height"></param>
	/// <param name="minDepth"></param>
	/// <param name="maxDepth"></param>
	/// <returns></returns>
	Matrix4x4 MakeViewportMatrix(float left, float top, float width, float height, float minDepth, float maxDepth);

	/// <summary>
	/// �]��
	/// </summary>
	/// <param name="theta"></param>
	/// <returns></returns>
	static float Cot(float theta);

	/// <summary>
	/// �������e�s��
	/// </summary>
	/// <param name="fovY"></param>
	/// <param name="aspectRatio"></param>
	/// <param name="nearClip"></param>
	/// <param name="farClip"></param>
	/// <returns></returns>
	Matrix4x4 MakePerspectiveFovMatrix(float fovY, float aspectRatio, float nearClip, float farClip);

	Matrix4x4 toDeviceSpace(Matrix4x4 worldViewProjectionMatrix, int32_t kWindowWidth, int32_t kWindowHeight);
#pragma endregion 





private:

};

