#include "ModelPlaneState.h"





void ModelPlaneState::Initialize(Model* state)
{
	resource_.Vertex = CreateResources::CreateBufferResource(sizeof(VertexData) * VertexSize);
	resource_.Material = CreateResources::CreateBufferResource(sizeof(Vector4));
	resource_.wvpResource = CreateResources::CreateBufferResource(sizeof(TransformationMatrix));
	resource_.BufferView = CreateResources::VertexCreateBufferView(sizeof(VertexData) * VertexSize, resource_.Vertex.Get(), VertexSize);
	resource_.Index = CreateResources::CreateBufferResource(sizeof(uint32_t) * IndexSize);
	resource_.IndexBufferView = CreateResources::IndexCreateBufferView(sizeof(uint32_t) * IndexSize, resource_.Index.Get());
	state;
}

void ModelPlaneState::Draw(Model* state, WorldTransform worldTransform, ViewProjection viewprojection)
{
	VertexData* vertexData = nullptr;
	Vector4* materialData = nullptr;
	//LightData* lightData = nullptr;
	uint32_t* indexData = nullptr;


	resource_.Vertex->Map(0, nullptr, reinterpret_cast<void**>(&vertexData));
	resource_.Material->Map(0, nullptr, reinterpret_cast<void**>(&materialData));
	resource_.Index->Map(0, nullptr, reinterpret_cast<void**>(&indexData));
	Vector4 pos = state->GetCenterPos();
	float size = state->GetSize();

	vertexData[0].position = { pos.x - size,pos.y + size,pos.z,pos.w };
	vertexData[1].position = { pos.x - size,pos.y - size,pos.z,pos.w };
	vertexData[2].position = { pos.x + size,pos.y + size,pos.z,pos.w };
	vertexData[3].position = { pos.x + size,pos.y - size,pos.z,pos.w };

	indexData[0] = 0; indexData[1] = 1; indexData[2] = 2;
	indexData[3] = 1; indexData[4] = 3; indexData[5] = 2;

	*materialData = state->GetColor();
	worldTransform.TransfarMatrix(resource_.wvpResource,viewprojection);
	
	CommandCall();

}



void ModelPlaneState::CommandCall()
{
	Commands commands = DirectXCommon::GetInstance()->GetCommands();
	SPSOProperty PSO = GraphicsPipelineManager::GetInstance()->GetPso().shape;


	commands.m_pList->SetGraphicsRootSignature(PSO.rootSignature.Get());
	commands.m_pList->SetPipelineState(PSO.GraphicsPipelineState.Get());

	commands.m_pList->IASetVertexBuffers(0, 1, &resource_.BufferView);
	commands.m_pList->IASetIndexBuffer(&resource_.IndexBufferView);

	//�`���ݒ�BPSO�ɐݒ肵�Ă�����̂Ƃ͂܂��ʁB�������̂�ݒ肷��ƍl���Ă����Ηǂ�
	commands.m_pList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//�}�e���A��CBuffer�̏ꏊ��ݒ�
	commands.m_pList->SetGraphicsRootConstantBufferView(0, resource_.Material->GetGPUVirtualAddress());

	//wvp�p��CBuffer�̏ꏊ��ݒ�
	commands.m_pList->SetGraphicsRootConstantBufferView(1, resource_.wvpResource->GetGPUVirtualAddress());

	//�`��(DrawCall/�h���[�R�[��)�B
	commands.m_pList->DrawIndexedInstanced(IndexSize, 1, 0, 0, 0);


}
