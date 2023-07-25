#pragma once

#include<windows.h>
#include<format>
#include<string>
#include <dxgidebug.h>
#include <dxcapi.h>
#include <d3d12.h>
#include <dxgi1_6.h>

#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"dxgi.lib")
#pragma comment(lib,"dxguid.lib")
#pragma comment(lib,"dxcompiler.lib")

#include<cassert>

#include"Log.h"
#include"DirectXSetup.h"


#include"Vector2.h"
#include"Vector3.h"
#include"Vector4.h"

#include"Matrix4x4.h"
#include"MatrixTransform.h"

#include"WorldTransform.h"

#define WHITE 0xFFFFFFFF
#define BLACK 0x000000FF


struct PSOProperty
{
	ID3D12PipelineState* GraphicsPipelineState = nullptr;
	ID3D12RootSignature* rootSignature = nullptr;
	ID3DBlob* signatureBlob = nullptr;
	ID3DBlob* errorBlob = nullptr;

};
struct  VertexData
{
	Vector4 position;
	Vector2 texcoord;

};


struct DXCProperty
{
	IDxcUtils* Utils;
	IDxcCompiler3* Compiler;

};

struct  Mode
{
	IDxcBlob* vertexBlob;
	IDxcBlob* pixelBlob;
};

struct Shaders
{
	Mode shape;
	Mode sprite;

};


struct ResourcePeroperty
{
	
   D3D12_VERTEX_BUFFER_VIEW BufferView;
   ID3D12Resource* Vertex;
   ID3D12Resource* Material;
   ID3D12Resource* wvpResource;
	

};

class Model 
{
public:
	Model();
	~Model();

	static Model*GetInstance();

#pragma region ������

	
	static void SetDevice(ID3D12Device* device_);
	static void SetCommands(Commands command);

	static void dxcInitialize();

	static void DefaultIncludeHandlerInitialize();


	/// <summary>
    /// �V�F�[�_�[�R���p�C���֐�
    /// </summary>
    /// <param name="filePath"></param>
    /// <param name="profile"></param>
    /// <param name="dxcUtils"></param>
    /// <param name="dxcCompiler"></param>
    /// <param name="includeHandler"></param>
    /// <returns></returns>
	static IDxcBlob* CompilerShader(
		const std::wstring& filePath,
		const wchar_t* profile,
		IDxcUtils* dxcUtils,
		IDxcCompiler3* dxcCompiler,
		IDxcIncludeHandler* includeHandler);

	/// <summary>
	/// ���ۂɃR���p�C������
	/// </summary>
	static void CompileShaders();

	/// <summary>
	/// �F�̐}�`��PSO����
	/// </summary>
	static void ShapeCreatePSO();


	/// <summary>
	/// �R���p�C�������V�F�[�_�[�̃����[�X
	/// </summary>
	static void ShaderRelease();



	/// <summary>
	/// PSO�Ȃǂ̃����[�X
	/// </summary>
	static void Finalize();


#pragma endregion


#pragma region �O�p�`


	/// <summary>
	/// Resource����
	/// </summary>
	/// <returns></returns>
	static ResourcePeroperty  CreateResource();


	/// <summary>
	/// �\��
	/// </summary>
	/// <param name="position"></param>
	/// <param name="Color"></param>
	/// <param name="Resource"></param>
	static void Draw(
		Vector3 position,
		float size,
		unsigned int Color, 
		WorldTransform worldTransform,
		ResourcePeroperty Resource);

	/// <summary>
	/// Resource�̉������
	/// </summary>
	/// <param name="Resource"></param>
	static void ResourceRelease(ResourcePeroperty Resource);

	
#pragma endregion

private:

	/// <summary>
    /// �F�ϊ�
    /// </summary>
    /// <param name="color"></param>
    /// <returns></returns>
	static Vector4 ColorCodeAdapter(unsigned int color);


	/// <summary>
	/// Resource���쐬
	/// </summary>
	/// <param name="device"></param>
	/// <param name="sizeInbyte"></param>
	/// <returns></returns>
	static ID3D12Resource* CreateBufferResource(ID3D12Device* device, size_t sizeInbyte);

	/// <summary>
	/// BufferView���쐬
	/// </summary>
	/// <param name="sizeInbyte"></param>
	/// <param name="Resource"></param>
	/// <returns></returns>
	static D3D12_VERTEX_BUFFER_VIEW CreateBufferView(size_t sizeInbyte, ID3D12Resource* Resource);


	/// <summary>
    /// ShapeCommands
    /// </summary>
    /// <param name="commands"></param>
	static void DrawCommand(
		Commands commands,
		ResourcePeroperty Resource,
		PSOProperty Shape);


	/// <summary>
	/// �p�C�v���C���̉��
	/// </summary>
	static void PSORelese(PSOProperty PSO);

	/// <summary>
	/// �V�F�[�_�[�̉��
	/// </summary>
	static void FancShaderRelease(Mode shader);


	ID3D12Device* device=nullptr;
	Commands commands;

	DXCProperty dxc;

	IDxcIncludeHandler* includeHandler = nullptr;
	Shaders shader ;

	PSOProperty Shape;
	PSOProperty Sprite;


	
};


