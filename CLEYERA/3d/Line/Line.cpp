#include"Line.h"

Line::Line()
{
}

Line::~Line()
{
}

void Line::Initialize(LinePloperty line, WorldTransform worldTransform)
{

	line_ = line;
	matworld = worldTransform.matWorld;
	const int VertexNum = 2;
	const int IndexNum = 1;
	resource_ = CreateResources::Vector4CreateResource(VertexNum,IndexNum);

}

void Line::Draw()
{

	VertexData* vertexData = nullptr;
	Vector4* MaterialData = nullptr;
	Matrix4x4* wvpData = nullptr;
	
	const int VertexNum = 2;

	resource_.Vertex->Map(0, nullptr, reinterpret_cast<void**>(&vertexData));
	resource_.Material->Map(0, nullptr, reinterpret_cast<void**>(&MaterialData));
	resource_.wvpResource->Map(0, nullptr, reinterpret_cast<void**>(&wvpData));

	ImGui::Begin("line");
	ImGui::SliderFloat3("sX", &line_.startPos.x, -10.0f, 10.0f);
	ImGui::SliderFloat3("eX", &line_.endpos.x, -10.0f, 10.0f);

	ImGui::End();

	//pos
	vertexData[0].position = line_.startPos;

	vertexData[1].position = line_.endpos;

	*MaterialData = line_.color;
	//matrix
	*wvpData = worldTransform_.matWorld;

	DrawCall(VertexNum);

}

void Line::Release()
{
	resource_.Material->Release();
	resource_.Vertex->Release();
	resource_.wvpResource->Release();

}

void Line::DrawCall(const int IndexNum)
{
	Commands commands = DirectXCommon::GetInstance()->GetCommands();
	PSOProperty PSO = GraphicsPipeline::GetInstance()->GetPSO().shape;

	commands.List->SetGraphicsRootSignature(PSO.rootSignature);
	commands.List->SetPipelineState(PSO.GraphicsPipelineState);

	commands.List->IASetVertexBuffers(0, 1, &resource_.BufferView);

	//形状を設定。PSOに設定しているものとはまた別。同じものを設定すると考えておけば良い
	commands.List->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_LINELIST);

	//materialBufferAddress
	commands.List->SetGraphicsRootConstantBufferView(0, resource_.Material->GetGPUVirtualAddress());
	//wvp用のCBufferの場所を設定
	commands.List->SetGraphicsRootConstantBufferView(1, resource_.wvpResource->GetGPUVirtualAddress());

	//IndexDrawCall
	commands.List->DrawInstanced(IndexNum,1,0,0);
}

