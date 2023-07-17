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

#include"../LogManager/Log.h"
#include"../DX/DirectXSetup.h"

#include"../Vector/Vector4.h"
#include"../Vector/Vector3.h"

#include"../Matrix/Matrix4x4.h"
#include"../Matrix/MatrixTransform.h"

#include"../TexManager/TexManager.h"
#include"../Vector/Vector2.h"


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


struct  Position
{
	Vector3 top;
	Vector3 left;
	Vector3 right;

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

#pragma region 初期化
	void Initialize();

	

	static void SetDevice(ID3D12Device* device_);
	static void SetCommands(Commands command);

	static void dxcInitialize();

	static void InitializeDfIncludeHandler();


	/// <summary>
    /// シェーダーコンパイル関数
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
	/// 実際にコンパイルする
	/// </summary>
	static void CompileShaders();

	/// <summary>
	/// 色の図形のPSO生成
	/// </summary>
	static void ShapeCreatePSO();

	/// <summary>
	/// 画像用のPSO
	/// </summary>
	static void SpriteCreatePSO();



	/// <summary>
	/// コンパイルしたシェーダーのリリース
	/// </summary>
	static void ShaderRelease();



	/// <summary>
	/// PSOなどのリリース
	/// </summary>
	static void Finalize();


#pragma endregion


#pragma region 三角形


	/// <summary>
	/// Resource生成
	/// </summary>
	/// <returns></returns>
	static ResourcePeroperty  CreateShapeResource();


	/// <summary>
	/// 表示
	/// </summary>
	/// <param name="position"></param>
	/// <param name="Color"></param>
	/// <param name="Resource"></param>
	static void ShapeDraw(
		Position position, unsigned int Color, 
		Matrix4x4 matrix,
		ResourcePeroperty Resource);

	/// <summary>
	/// Resourceの解放処理
	/// </summary>
	/// <param name="Resource"></param>
	static void ShapeResourceRelease(ResourcePeroperty Resource);

	
#pragma endregion

#pragma region 円



#pragma endregion

#pragma region 画像表示

	static ResourcePeroperty CreateTriangleSpriteResource();

	static void TriangleSpriteDraw(
		Position position, unsigned int color,
		Matrix4x4 worldTransform,
		ResourcePeroperty Resource,
		texResourceProperty tex);

	static void TriangleSpriteResourceRelease(ResourcePeroperty &Resource,texResourceProperty &tex);
	
	static void Triangle3DSpriteDraw(
		Position position, unsigned int color,
		Matrix4x4 worldTransform,
		ResourcePeroperty Resource,
		texResourceProperty tex);
#pragma endregion


private:

	/// <summary>
    /// 色変換
    /// </summary>
    /// <param name="color"></param>
    /// <returns></returns>
	static Vector4 ColorCodeAdapter(unsigned int color);


	/// <summary>
	/// Resourceを作成
	/// </summary>
	/// <param name="device"></param>
	/// <param name="sizeInbyte"></param>
	/// <returns></returns>
	static ID3D12Resource* CreateBufferResource(ID3D12Device* device, size_t sizeInbyte);

	/// <summary>
	/// BufferViewを作成
	/// </summary>
	/// <param name="sizeInbyte"></param>
	/// <param name="Resource"></param>
	/// <returns></returns>
	static D3D12_VERTEX_BUFFER_VIEW CreateBufferView(size_t sizeInbyte, ID3D12Resource* Resource);


	/// <summary>
    /// ShapeCommands
    /// </summary>
    /// <param name="commands"></param>
	static void ShapeDrawCommands(
		Commands commands,
		ResourcePeroperty Resource,
		PSOProperty Shape);

	/// <summary>
    /// SpriteCommands
    /// </summary>
    /// <param name="commands"></param>
	static void SpriteDrawCommands(
		ResourcePeroperty Resource, 
		texResourceProperty tex, 
		Commands commands, PSOProperty PSO);

	/// <summary>
	/// パイプラインの解放
	/// </summary>
	static void PSORelese(PSOProperty PSO);

	/// <summary>
	/// シェーダーの解放
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


