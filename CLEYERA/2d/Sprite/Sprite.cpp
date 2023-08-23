#include "Sprite.h"

Sprite::Sprite()
{
}

Sprite::~Sprite()
{
}

void Sprite::Initialize(Vector2 leftpos, float size,WorldTransform worldTransform, texResourceProperty texResource)
{
	worldTransform_ = worldTransform;
	tex_ = texResource;

	const int NumVertex = 4;
	const int NumIndex = 6;

	pos_.leftTop = { leftpos.x,leftpos.y,0.0f,1.0f };
	pos_.rightTop = { leftpos.x + size,leftpos.y,0.0f,1.0f };
	pos_.leftBottom = { leftpos.x,leftpos.y + size,0.0f,1.0f };
	pos_.rightBottom = { leftpos.x + size,leftpos.y + size,0.0f,1.0f };

	resource_ = CreateResource(NumVertex,NumIndex);
}

void Sprite::TransferMatrix(Matrix4x4 m)
{
	worldTransform_.matWorld = m;
}

void Sprite::SetTexProperty(texResourceProperty NewTex)
{
	tex_ = NewTex;
}

void Sprite::Draw()
{
	VertexData* vertexData = nullptr;
	Material* MaterialData = nullptr;
	Matrix4x4* wvpData = nullptr;
	uint32_t* indexData = nullptr;

	
	const int IndexNum = 6;

	resource_.Vertex->Map(0, nullptr, reinterpret_cast<void**>(&vertexData));
	resource_.Material->Map(0, nullptr, reinterpret_cast<void**>(&MaterialData));
	resource_.wvpResource->Map(0, nullptr, reinterpret_cast<void**>(&wvpData));
	resource_.Index->Map(0, nullptr, reinterpret_cast<void**>(&indexData));
	
	//index
	//1
	indexData[0] = 0; indexData[1] = 1; indexData[2] = 2;
	//2
	indexData[3] = 1; indexData[4] = 3; indexData[5] = 2;

	//pos
	vertexData[0].position = { pos_.leftBottom };
	vertexData[0].texcoord = { 0.0f,1.0f };
	
	vertexData[1].position = { pos_.leftTop };
    vertexData[1].texcoord = { 0.0f,0.0f };
	
	vertexData[2].position = {pos_.rightBottom};
	vertexData[2].texcoord = { 1.0f,1.0f };

	vertexData[3].position = {pos_.rightTop};
	vertexData[3].texcoord = { 1.0f,0.0f };


	//material
	MaterialData->color = color_;

	ImGui::Begin("sprite");
	ImGui::Text("uv");
	ImGui::SliderFloat3("t", &uvTranformSprite.translate.x, -4.0f, 4.0f);
	ImGui::SliderFloat3("s", &uvTranformSprite.Scale.x, -4.0f, 4.0f);
	ImGui::SliderFloat3("r", &uvTranformSprite.rotate.x, -4.0f, 4.0f);
	ImGui::End();
	
	Matrix4x4 m = MatrixTransform::MakeAffineMatrix(uvTranformSprite.Scale, uvTranformSprite.rotate, uvTranformSprite.translate);
	MaterialData->uvTransform = m;



	//matrix
	worldTransform_.matWorld=Camera::worldOthographicMatrix(worldTransform_.matWorld);
	*wvpData = worldTransform_.matWorld;

	//call
	CommandCall(IndexNum);

}

void Sprite::Release()
{
	Sprite::Releace(resource_.Material);
	Sprite::Releace(resource_.Vertex);
	Sprite::Releace(resource_.wvpResource);
	Sprite::Releace(resource_.Index);
}

void Sprite::Releace(ID3D12Resource* resource)
{
	resource->Release();
}

void Sprite::CommandCall(const int Num)
{
	Commands commands = DirectXCommon::GetInstance()->GetCommands();
	PSOProperty PSO = GraphicsPipeline::GetInstance()->GetPSO().sprite;

	commands.List->SetGraphicsRootSignature(PSO.rootSignature);
	commands.List->SetPipelineState(PSO.GraphicsPipelineState);

	commands.List->IASetVertexBuffers(0, 1, &resource_.BufferView);
	commands.List->IASetIndexBuffer(&resource_.IndexBufferView);

	//形状を設定。PSOに設定しているものとはまた別。同じものを設定すると考えておけば良い
	commands.List->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	
	//materialBufferAddress
	commands.List->SetGraphicsRootConstantBufferView(0, resource_.Material->GetGPUVirtualAddress());
	//wvp用のCBufferの場所を設定
	commands.List->SetGraphicsRootConstantBufferView(1, resource_.wvpResource->GetGPUVirtualAddress());
	//tex
	commands.List->SetGraphicsRootDescriptorTable(2, tex_.SrvHandleGPU);

	//IndexDrawCall
	commands.List->DrawIndexedInstanced(Num, 1, 0, 0, 0);
	
}


ResourcePeroperty Sprite::CreateResource(const int NumVertex, const int NumIndex)
{
	 ResourcePeroperty result;

	 result.Vertex = CreateResources::CreateBufferResource(sizeof(VertexData) * NumVertex);
	 result.Material = CreateResources::CreateBufferResource(sizeof(Material));
	 result.Index = CreateResources::CreateBufferResource(sizeof(uint32_t) * NumIndex);
	 result.wvpResource = CreateResources::CreateBufferResource(sizeof(Matrix4x4));
	 result.BufferView = CreateResources::VertexCreateBufferView(sizeof(VertexData) * NumVertex, result.Vertex, NumVertex);
	 result.IndexBufferView = CreateResources::IndexCreateBufferView(sizeof(uint32_t) * NumIndex, result.Index);

	 return result;
}
