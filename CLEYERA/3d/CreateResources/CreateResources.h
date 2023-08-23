#pragma once
#include"GraphicsPipeline.h"
#include"DirectXCommon.h"
#include"Vector4.h"
#include"Matrix4x4.h"
#include"Vector2.h"
#include"Vector3.h"


struct  VertexData
{
	Vector4 position;
	Vector2 texcoord;
	Vector3 normal;
};

struct LightData
{
	Vector4 color;
	Vector3 direction;
	float intensity;
	
};

struct TransformationMatrix {
	Matrix4x4 WVP;
	Matrix4x4 world;
};

struct ResourcePeroperty
{
	D3D12_VERTEX_BUFFER_VIEW BufferView;
	D3D12_INDEX_BUFFER_VIEW IndexBufferView;
	ID3D12Resource* Index;
	ID3D12Resource* Vertex;
	ID3D12Resource* Material;
	ID3D12Resource* wvpResource;
	ID3D12Resource* Light;
};



class CreateResources
{
public:


	static CreateResources* GetInstance();

	/// <summary>
    /// ResourceとbufferViewの生成
    /// </summary>
    /// <returns></returns>
	static ResourcePeroperty Vector4CreateResource(const int VertexNum, const int IndexNum);

	static ResourcePeroperty VertexDataCreateResource(const int Num);




	static void Release(ID3D12Resource* resource);

	/// <summary>
	/// Resourceを作成
	/// </summary>
	/// <param name="device"></param>
	/// <param name="sizeInbyte"></param>
	/// <returns></returns>
	static ID3D12Resource* CreateBufferResource( size_t sizeInbyte);

	/// <summary>
	/// BufferViewを作成
	/// </summary>
	/// <param name="sizeInbyte"></param>
	/// <param name="Resource"></param>
	/// <returns></returns>
	static D3D12_VERTEX_BUFFER_VIEW VertexCreateBufferView(size_t sizeInbyte, ID3D12Resource* Resource, int size);
	/// <summary>
    /// indexのBufferView
    /// </summary>
    /// <param name="NumVertex"></param>
    /// <returns></returns>
	static D3D12_INDEX_BUFFER_VIEW IndexCreateBufferView(size_t sizeInbyte, ID3D12Resource* Resource);

private:


};

