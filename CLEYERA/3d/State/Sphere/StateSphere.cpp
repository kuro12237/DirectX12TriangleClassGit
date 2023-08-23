#include "StateSphere.h"

void StateSphere::Initialize(Vector4 pos, float size, WorldTransform worldTransform, texResourceProperty tex)
{
	resource_ = CreateResources::VertexDataCreateResource(VertexNum * VertexNum * 4);

	resource_.Index = CreateResources::CreateBufferResource(sizeof(uint32_t)*VertexNum * VertexNum * 6);
	resource_.IndexBufferView = CreateResources::IndexCreateBufferView(sizeof(uint32_t) * VertexNum * VertexNum * 6,resource_.Index);

	centerPos_ = pos;
	size_ = size;
	workdTransform_ = worldTransform;
	tex_ = tex;
}

void StateSphere::Draw()
{
	VertexData* vertexData = nullptr;
	Vector4* materialData = nullptr;
	TransformationMatrix* matrixData = nullptr;
	LightData* lightData = nullptr;
	uint32_t* indexData = nullptr;

	
	resource_.Vertex->Map(0, nullptr, reinterpret_cast<void**>(&vertexData));
	resource_.Material->Map(0, nullptr, reinterpret_cast<void**>(&materialData));
	resource_.wvpResource->Map(0, nullptr, reinterpret_cast<void**>(&matrixData));
	resource_.Light->Map(0, nullptr, reinterpret_cast<void**>(&lightData));
	resource_.Index->Map(0, nullptr, reinterpret_cast<void**>(&indexData));

#pragma region ‹…‚ÌÀ•Wˆ—
	const float LON_EVERY = float(std::numbers::pi) * 2.0f / float(VertexNum);

	const float LAT_EVERY = float(std::numbers::pi) / float(VertexNum);
	{
		for (int latIndex = 0; latIndex < VertexNum; ++latIndex) {
			//ƒÆ
			float lat = -float(std::numbers::pi) / 2.0f + LAT_EVERY * latIndex;
			
			for (int lonIndex = 0; lonIndex < VertexNum; ++lonIndex) {
				
				uint32_t Vstart = (latIndex * VertexNum + lonIndex) * 4;
				uint32_t Istart = (latIndex * VertexNum + lonIndex) * 6;
				
				float u = float(lonIndex) / float(VertexNum);
				float v = 1.0f - float(latIndex) / float(VertexNum);

				float lon = lonIndex * LON_EVERY;
				float length = 1.0f / VertexNum;

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

	vertexData[0].normal = { 0.0f,0.0f,0.5f };

	matrixData->WVP = workdTransform_.matWorld;
	matrixData->world = MatrixTransform::Identity();

	lightData->color = { 1.0f,1.0f,1.0f,1.0f };
	lightData->direction = directionPos;

	ImGui::Begin("LightDirection");
	ImGui::SliderFloat3("pos", &directionPos.x, -1.0f, 1.0f);
	ImGui::End();

	lightData->intensity = 1.0f;

	*materialData = color_;

	CommandCall();
}

void StateSphere::Release()
{
	CreateResources::Release(resource_.wvpResource);
	CreateResources::Release(resource_.Material);
	CreateResources::Release(resource_.Vertex);
	CreateResources::Release(resource_.Light);
}

void StateSphere::TransferMatrix(Matrix4x4 m)
{
	workdTransform_.matWorld = m;
}

void StateSphere::SetTexProperty(texResourceProperty NewTex)
{
	tex_ = NewTex;
}

Matrix4x4 StateSphere::GetWorldTransform()
{
	return workdTransform_.matWorld;
}

void StateSphere::CommandCall()
{
	Commands commands = DirectXCommon::GetInstance()->GetCommands();
	PSOProperty PSO = GraphicsPipeline::GetInstance()->GetPSO().indirectLight;

	commands.List->SetGraphicsRootSignature(PSO.rootSignature);
	commands.List->SetPipelineState(PSO.GraphicsPipelineState);
	
	commands.List->IASetVertexBuffers(0, 1, &resource_.BufferView);
	commands.List->IASetIndexBuffer(&resource_.IndexBufferView);

	commands.List->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	commands.List->SetGraphicsRootConstantBufferView(0, resource_.Material->GetGPUVirtualAddress());

	commands.List->SetGraphicsRootDescriptorTable(2, tex_.SrvHandleGPU);

	commands.List->SetGraphicsRootConstantBufferView(1, resource_.wvpResource->GetGPUVirtualAddress());
	commands.List->SetGraphicsRootConstantBufferView(3, resource_.Light->GetGPUVirtualAddress());

	commands.List->DrawIndexedInstanced(VertexNum * VertexNum * 6, 1, 0, 0, 0);
}

//ƒƒ‚
//CreateReosuorce‚Í‚Ü‚Æ‚ß‚È‚¢‚Ù‚¤‚ª‚¢‚¢‚©‚à‚µ‚ê‚È‚¢