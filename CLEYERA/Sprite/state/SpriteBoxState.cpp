#include "SpriteBoxState.h"

void SpriteBoxState::Initialize(Sprite* state)
{
	resource_.Vertex = CreateResources::CreateBufferResource(sizeof(VertexData) * VertexSize);
	resource_.Material = CreateResources::CreateBufferResource(sizeof(Material));
	resource_.wvpResource = CreateResources::CreateBufferResource(sizeof(TransformationMatrix));
	resource_.BufferView = CreateResources::VertexCreateBufferView(sizeof(VertexData) * VertexSize, resource_.Vertex.Get(), VertexSize);
	resource_.Index = CreateResources::CreateBufferResource(sizeof(uint32_t) * IndexSize);
	resource_.IndexBufferView = CreateResources::IndexCreateBufferView(sizeof(uint32_t) * IndexSize, resource_.Index.Get());
	state;
}

void SpriteBoxState::Draw(Sprite* state, WorldTransform worldTransform)
{
	VertexData* vertexData = nullptr;
	Material* materialData = nullptr;
	uint32_t* indexData = nullptr;
	resource_.Vertex->Map(0, nullptr, reinterpret_cast<void**>(&vertexData));
	resource_.Material->Map(0, nullptr, reinterpret_cast<void**>(&materialData));
	resource_.Index->Map(0, nullptr, reinterpret_cast<void**>(&indexData));
	Vector2 pos = state->GetPos();
	Vector2 size = state->GetSize();

	vertexData[0].position = { pos.x,pos.y+size.y,1,1 };
	vertexData[0].texcoord = { 0.0f,1.0f };
	
	vertexData[1].position = { pos.x ,pos.y,1,1 };
	vertexData[1].texcoord = { 0.0f,0.0f };


	vertexData[2].position = { pos.x + size.x,pos.y+size.y,1,1 };
	vertexData[2].texcoord = { 1.0f,1.0f };

	vertexData[3].position = { pos.x + size.x,pos.y,1,1 };
	vertexData[3].texcoord = { 1.0f,0.0f };


	indexData[0] = 0; indexData[1] = 1; indexData[2] = 2;
	indexData[3] = 1; indexData[4] = 3; indexData[5] = 2;

	materialData->color = state->GetColor();
	materialData->uvTransform = MatrixTransform::AffineMatrix(state->GetuvScale(), state->GetuvRotate(), state->GetuvTranslate());


	ViewProjection viewprojection = {};

	worldTransform.TransfarMatrix(resource_.wvpResource,viewprojection,OrthographicMatrix);

	CommandCall(state->GetTexHandle());

}
void SpriteBoxState::CommandCall(uint32_t texHandle)
{
	Commands commands = DirectXCommon::GetInstance()->GetCommands();

	SPSOProperty PSO = {};

	if (texHandle == 0)
	{
		PSO = GraphicsPipelineManager::GetInstance()->GetPso().shape;
	}
	else if (!texHandle == 0)
	{
		PSO = GraphicsPipelineManager::GetInstance()->GetPso().Sprite;
	}


	commands.m_pList->SetGraphicsRootSignature(PSO.rootSignature.Get());
	commands.m_pList->SetPipelineState(PSO.GraphicsPipelineState.Get());

	commands.m_pList->IASetVertexBuffers(0, 1, &resource_.BufferView);
	commands.m_pList->IASetIndexBuffer(&resource_.IndexBufferView);

	//形状を設定。PSOに設定しているものとはまた別。同じものを設定すると考えておけば良い
	commands.m_pList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//マテリアルCBufferの場所を設定
	commands.m_pList->SetGraphicsRootConstantBufferView(0, resource_.Material->GetGPUVirtualAddress());

	//wvp用のCBufferの場所を設定
	commands.m_pList->SetGraphicsRootConstantBufferView(1, resource_.wvpResource->GetGPUVirtualAddress());

	if (!texHandle == 0)
	{
		TextureManager::texCommand(texHandle);
	}

	//描画(DrawCall/ドローコール)。
	commands.m_pList->DrawIndexedInstanced(IndexSize, 1, 0, 0, 0);


}
