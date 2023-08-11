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
	//�������ނ��߂̃A�h���X���擾
	resource_.Vertex->Map(0, nullptr, reinterpret_cast<void**>(&vertexData));
	resource_.Material->Map(0, nullptr, reinterpret_cast<void**>(&MaterialData));
	resource_.wvpResource->Map(0, nullptr, reinterpret_cast<void**>(&wvpData));


	//���W
	//����
	vertexData[0] = //{ -0.5f,0.0f,0.0f,1.0f };
	{ CenterPos_.x - size_,CenterPos_.y - size_,CenterPos_.z,CenterPos_.w };

	//��
	vertexData[1] = { CenterPos_.x,CenterPos_.y + size_,CenterPos_.z,CenterPos_.w };

	//�E��
	vertexData[2] ={ CenterPos_.x + size_,CenterPos_.y - size_,CenterPos_.z,CenterPos_.w };
	//�}�e���A��
	Vector4 colorData = Color_;

	*MaterialData = colorData;

	//�s��̕ϊ�

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

	//�`���ݒ�BPSO�ɐݒ肵�Ă�����̂Ƃ͂܂��ʁB�������̂�ݒ肷��ƍl���Ă����Ηǂ�
	commands.List->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//�}�e���A��CBuffer�̏ꏊ��ݒ�
	commands.List->SetGraphicsRootConstantBufferView(0, resource.Material->GetGPUVirtualAddress());

	//wvp�p��CBuffer�̏ꏊ��ݒ�
	commands.List->SetGraphicsRootConstantBufferView(1, resource.wvpResource->GetGPUVirtualAddress());

	//�`��(DrawCall/�h���[�R�[��)�B
	commands.List->DrawInstanced(3, 1, 0, 0);


}






