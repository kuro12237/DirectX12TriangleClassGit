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

struct ResourcePeroperty
{
	D3D12_VERTEX_BUFFER_VIEW BufferView;
	ID3D12Resource* Vertex;
	ID3D12Resource* Material;
	ID3D12Resource* wvpResource;
	ID3D12Resource* Light;
};

struct TransformationMatrix {
	Matrix4x4 WVP;
	Matrix4x4 world;
};

class CreateResources
{
public:


	static CreateResources* GetInstance();

	/// <summary>
    /// ResourceÇ∆bufferViewÇÃê∂ê¨
    /// </summary>
    /// <returns></returns>
	static ResourcePeroperty Vector4CreateResource(const int Num);

	static ResourcePeroperty VertexDataCreateResource(const int Num);

	static void Release(ID3D12Resource* resource);

	/// <summary>
	/// ResourceÇçÏê¨
	/// </summary>
	/// <param name="device"></param>
	/// <param name="sizeInbyte"></param>
	/// <returns></returns>
	static ID3D12Resource* CreateBufferResource(ID3D12Device* device, size_t sizeInbyte);

	/// <summary>
	/// BufferViewÇçÏê¨
	/// </summary>
	/// <param name="sizeInbyte"></param>
	/// <param name="Resource"></param>
	/// <returns></returns>
	static D3D12_VERTEX_BUFFER_VIEW CreateBufferView(size_t sizeInbyte, ID3D12Resource* Resource, int size);

private:


};

