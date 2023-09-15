#include "ModelLineState.h"

void ModelLineState::Initialize(Model* state)
{

	resource_.Vertex = CreateResources::CreateBufferResource(sizeof(VertexData)*VertexSize);
	resource_.BufferView = CreateResources::VertexCreateBufferView(sizeof(VertexData) * VertexSize, resource_.Vertex.Get(), VertexSize);
	resource_.Material = CreateResources::CreateBufferResource(sizeof(Vector4));
	resource_.wvpResource = CreateResources::CreateBufferResource(sizeof(TransformationMatrix));
	state;

}

void ModelLineState::Draw(Model* state, WorldTransform worldTransform, ViewProjection viewprojection)
{

	worldTransform;
	state;
	VertexData* vertexData = nullptr;
	Vector4* materialData = nullptr;

	resource_.Vertex->Map(0, nullptr, reinterpret_cast<void**>(&vertexData));
	resource_.Material->Map(0, nullptr, reinterpret_cast<void**>(&materialData));


	vertexData[0].position = {state->GetStartPos().x,state->GetStartPos().y,state->GetStartPos().z,state->GetStartPos().w };
	vertexData[1].position = {state->GetEndPos().x,state->GetEndPos().y,state->GetEndPos().z,state->GetEndPos().w};

	*materialData = state->GetColor();
	worldTransform.TransfarMatrix(resource_.wvpResource,viewprojection);

	CommandCall();


}

void ModelLineState::CommandCall()
{
	Commands commands = DirectXCommon::GetInstance()->GetCommands();
	SPSOProperty PSO = GraphicsPipelineManager::GetInstance()->GetPso().Line;


	commands.m_pList->SetGraphicsRootSignature(PSO.rootSignature.Get());
	commands.m_pList->SetPipelineState(PSO.GraphicsPipelineState.Get());

	commands.m_pList->IASetVertexBuffers(0, 1, &resource_.BufferView);
	commands.m_pList->IASetIndexBuffer(&resource_.IndexBufferView);

	//形状を設定。PSOに設定しているものとはまた別。同じものを設定すると考えておけば良い
	commands.m_pList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_LINELIST);

	//マテリアルCBufferの場所を設定
	commands.m_pList->SetGraphicsRootConstantBufferView(0, resource_.Material->GetGPUVirtualAddress());

	//wvp用のCBufferの場所を設定
	commands.m_pList->SetGraphicsRootConstantBufferView(1, resource_.wvpResource->GetGPUVirtualAddress());

	//描画(DrawCall/ドローコール)。
	commands.m_pList->DrawInstanced(VertexSize, 1, 0, 0);
}
