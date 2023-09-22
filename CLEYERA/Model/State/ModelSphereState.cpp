#include "ModelSphereState.h"



void ModelSphereState::Initialize(Model* state)
{
	int v = VertexNum * VertexNum * 4;
	resource_.Vertex = CreateResources::CreateBufferResource(sizeof(VertexData) * v);
	resource_.Material = CreateResources::CreateBufferResource(sizeof(Material));
	resource_.wvpResource = CreateResources::CreateBufferResource(sizeof(TransformationMatrix));
	resource_.BufferView = CreateResources::VertexCreateBufferView(sizeof(VertexData) * v, resource_.Vertex.Get(), v);
	if (state->GetUseLight()!=NONE)
	{
		resource_.Light = CreateResources::CreateBufferResource(sizeof(LightData));
	}

	uint32_t i = VertexNum * VertexNum * 6;
	resource_.Index = CreateResources::CreateBufferResource(sizeof(uint32_t) * i);
	resource_.IndexBufferView = CreateResources::IndexCreateBufferView(sizeof(uint32_t) * i, resource_.Index.Get());
	state;
}

void ModelSphereState::Draw(Model* state, WorldTransform worldTransform, ViewProjection viewprojection)
{
	VertexData* vertexData = nullptr;
	Material* materialData = nullptr;
	uint32_t* indexData = nullptr;


	resource_.Vertex->Map(0, nullptr, reinterpret_cast<void**>(&vertexData));
	resource_.Material->Map(0, nullptr, reinterpret_cast<void**>(&materialData));
	resource_.Index->Map(0, nullptr, reinterpret_cast<void**>(&indexData));

#pragma region 球の座標処理
	const float LON_EVERY = float(std::numbers::pi) * 2.0f / float(VertexNum);

	const float LAT_EVERY = float(std::numbers::pi) / float(VertexNum);
	{
		for (int latIndex = 0; latIndex < VertexNum; ++latIndex) {
			//θ
			float lat = -float(std::numbers::pi) / 2.0f + LAT_EVERY * latIndex;

			for (int lonIndex = 0; lonIndex < VertexNum; ++lonIndex) {

				uint32_t Vstart = (latIndex * VertexNum + lonIndex) * 4;
				uint32_t Istart = (latIndex * VertexNum + lonIndex) * 6;

				float u = float(lonIndex) / float(VertexNum);
				float v = 1.0f - float(latIndex) / float(VertexNum);

				float lon = lonIndex * LON_EVERY;
				float length = 1.0f / VertexNum;

				float size_ = state->GetSize();
				//
				vertexData[Vstart].position.x = size_ * (cos(lat) * cos(lon));
				vertexData[Vstart].position.y = size_ * (sin(lat));
				vertexData[Vstart].position.z = size_ * (cos(lat) * sin(lon));
				vertexData[Vstart].position.w = 1.0f;
				vertexData[Vstart].texcoord.x = u;
				vertexData[Vstart].texcoord.y = v + length;
				vertexData[Vstart].normal.x = vertexData[Vstart].position.x;
				vertexData[Vstart].normal.y = vertexData[Vstart].position.y;
				vertexData[Vstart].normal.z = vertexData[Vstart].position.z;

				//
				vertexData[Vstart + 1].position.x = size_ * (cos(lat + LAT_EVERY)) * cos(lon);
				vertexData[Vstart + 1].position.y = size_ * (sin(lat + LAT_EVERY));
				vertexData[Vstart + 1].position.z = size_ * (cos(lat + LAT_EVERY)) * sin(lon);
				vertexData[Vstart + 1].position.w = 1.0f;
				vertexData[Vstart + 1].texcoord.x = u;
				vertexData[Vstart + 1].texcoord.y = v;
				vertexData[Vstart + 1].normal.x = vertexData[Vstart + 1].position.x;
				vertexData[Vstart + 1].normal.y = vertexData[Vstart + 1].position.y;
				vertexData[Vstart + 1].normal.z = vertexData[Vstart + 1].position.z;

				//
				vertexData[Vstart + 2].position.x = size_ * (cos(lat) * cos(lon + LON_EVERY));
				vertexData[Vstart + 2].position.y = size_ * (sin(lat));
				vertexData[Vstart + 2].position.z = size_ * (cos(lat) * sin(lon + LON_EVERY));
				vertexData[Vstart + 2].position.w = 1.0f;
				vertexData[Vstart + 2].texcoord.x = u + length;
				vertexData[Vstart + 2].texcoord.y = v + length;
				vertexData[Vstart + 2].normal.x = vertexData[Vstart + 2].position.x;
				vertexData[Vstart + 2].normal.y = vertexData[Vstart + 2].position.y;
				vertexData[Vstart + 2].normal.z = vertexData[Vstart + 2].position.z;

				//
				vertexData[Vstart + 3].position.x = size_ * (cos(lat + LAT_EVERY) * cos(lon + LON_EVERY));
				vertexData[Vstart + 3].position.y = size_ * (sin(lat + LAT_EVERY));
				vertexData[Vstart + 3].position.z = size_ * (cos(lat + LAT_EVERY) * sin(lon + LON_EVERY));
				vertexData[Vstart + 3].position.w = 1.0f;
				vertexData[Vstart + 3].texcoord.x = u + length;
				vertexData[Vstart + 3].texcoord.y = v;
				vertexData[Vstart + 3].normal.x = vertexData[Vstart + 3].position.x;
				vertexData[Vstart + 3].normal.y = vertexData[Vstart + 3].position.y;
				vertexData[Vstart + 3].normal.z = vertexData[Vstart + 3].position.z;


				indexData[Istart] = Vstart;
				indexData[Istart + 1] = Vstart + 1;
				indexData[Istart + 2] = Vstart + 2;

				indexData[Istart + 3] = Vstart + 1;
				indexData[Istart + 4] = Vstart + 3;
				indexData[Istart + 5] = Vstart + 2;
			}
		}
	}
#pragma endregion

	worldTransform.TransfarMatrix(resource_.wvpResource,viewprojection);
	materialData->color = state->GetColor();
	materialData->uvTransform = MatrixTransform::AffineMatrix(state->GetuvScale(), state->GetuvRotate(), state->GetuvTranslate());
	if (state->GetUseLight()!=NONE)
	{
		LightData* lightData = nullptr;
		resource_.Light->Map(0, nullptr, reinterpret_cast<void**>(&lightData));
		
		lightData->color = { 1.0f,1.0f,1.0f,1.0f };
		lightData->direction = { 0.0f,-1.0f,0.0f };
		lightData->intensity = 1.0f;

	}

	CommandCall(state);
}

void ModelSphereState::CommandCall(Model*state)
{

	Commands commands = DirectXCommon::GetInstance()->GetCommands();
	SPSOProperty PSO = {};

	
	PSO = GraphicsPipelineManager::GetInstance()->GetPso().shape;

	//テクスチャがある場合
	if (!state->GetTexHandle() == 0) {
		PSO = GraphicsPipelineManager::GetInstance()->GetPso().Sprite;
	}
	if (state->GetUseLight() == HARF_LAMBERT)
	{
		PSO = GraphicsPipelineManager::GetInstance()->GetPso().Herf_Lambert;
	}
	commands.m_pList->SetGraphicsRootSignature(PSO.rootSignature.Get());
	commands.m_pList->SetPipelineState(PSO.GraphicsPipelineState.Get());

	commands.m_pList->IASetVertexBuffers(0, 1, &resource_.BufferView);
	commands.m_pList->IASetIndexBuffer(&resource_.IndexBufferView);

	commands.m_pList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	commands.m_pList->SetGraphicsRootConstantBufferView(0, resource_.Material->GetGPUVirtualAddress());

	if (!state->GetTexHandle() == 0)
	{
		TextureManager::texCommand(state->GetTexHandle());
	}
	commands.m_pList->SetGraphicsRootConstantBufferView(1, resource_.wvpResource->GetGPUVirtualAddress());
	if (state->GetUseLight()!=NONE)
	{
		commands.m_pList->SetGraphicsRootConstantBufferView(3, resource_.Light->GetGPUVirtualAddress());
	}

	commands.m_pList->DrawIndexedInstanced(VertexNum * VertexNum * 6, 1, 0, 0, 0);
}
