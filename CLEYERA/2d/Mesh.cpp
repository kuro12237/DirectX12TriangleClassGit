#include"Mesh.h"


Mesh::Mesh()
{
}

Mesh::~Mesh()
{
}


Mesh* Mesh::GetInstance()
{
	static Mesh instance;
	return &instance;
}

void Mesh::Initialize(WorldTransform worldTransform,Vector4 pos,Vector4 Color)
{
	const int Num = 3;
	resource_ = CreateResources::Vector4CreateResource(Num);
	Color_ = Color;
	CenterPos_ = pos;
	worldTransform_ = worldTransform;
}

void Mesh::TransferMatrix(Matrix4x4 m)
{
	worldTransform_.matWorld = m;
}


void Mesh::Draw()
{
	Vector4* vertexData = nullptr;
	Vector4* MaterialData = nullptr;
	Matrix4x4* wvpData = nullptr;
	//書き込むためのアドレスを取得
	resource_.Vertex->Map(0, nullptr, reinterpret_cast<void**>(&vertexData));
	resource_.Material->Map(0, nullptr, reinterpret_cast<void**>(&MaterialData));
	resource_.wvpResource->Map(0, nullptr, reinterpret_cast<void**>(&wvpData));


	//座標
	//左下
	vertexData[0] = //{ -0.5f,0.0f,0.0f,1.0f };
	{ CenterPos_.x - size_,CenterPos_.y - size_,CenterPos_.z,CenterPos_.w };

	//上
	vertexData[1] = { CenterPos_.x,CenterPos_.y + size_,CenterPos_.z,CenterPos_.w };

	//右上
	vertexData[2] ={ CenterPos_.x + size_,CenterPos_.y - size_,CenterPos_.z,CenterPos_.w };
	//マテリアル
	Vector4 colorData = Color_;

	*MaterialData = colorData;

	//行列の変換

	*wvpData = worldTransform_.matWorld;

	Mesh::ComanndCall(resource_);

}

void Mesh::Release()
{
	CreateResources::Release(resource_.wvpResource);
	CreateResources::Release(resource_.Material);
	CreateResources::Release(resource_.Vertex);
}

void Mesh::ComanndCall(ResourcePeroperty resource)
{
	Commands commands = DirectXCommon::GetInstance()->GetCommands();
	PSOProperty PSO = GraphicsPipeline::GetInstance()->GetPSO().shape;


	commands.List->SetGraphicsRootSignature(PSO.rootSignature);
	commands.List->SetPipelineState(PSO.GraphicsPipelineState);//

	commands.List->IASetVertexBuffers(0, 1, &resource.BufferView);

	//形状を設定。PSOに設定しているものとはまた別。同じものを設定すると考えておけば良い
	commands.List->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//マテリアルCBufferの場所を設定
	commands.List->SetGraphicsRootConstantBufferView(0, resource.Material->GetGPUVirtualAddress());

	//wvp用のCBufferの場所を設定
	commands.List->SetGraphicsRootConstantBufferView(1, resource.wvpResource->GetGPUVirtualAddress());

	//描画(DrawCall/ドローコール)。
	commands.List->DrawInstanced(3, 1, 0, 0);


}






