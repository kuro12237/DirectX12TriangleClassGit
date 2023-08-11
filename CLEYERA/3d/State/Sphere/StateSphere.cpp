#include "StateSphere.h"

void StateSphere::Initialize(Vector4 pos, float size, WorldTransform worldTransform, texResourceProperty tex)
{
	resource_ = CreateResources::VertexDataCreateResource(VertexNum * VertexNum * 6);

	centerPos_ = pos;
	size_ = size;
	workdTransform_ = worldTransform;
	tex_ = tex;
}

void StateSphere::Draw()
{
	VertexData* vertexData = nullptr;
	Vector4* MaterialData = nullptr;
	Matrix4x4* wvpData = nullptr;

	//書き込むためのアドレスを取得
	resource_.Vertex->Map(0, nullptr, reinterpret_cast<void**>(&vertexData));
	resource_.Material->Map(0, nullptr, reinterpret_cast<void**>(&MaterialData));
	resource_.wvpResource->Map(0, nullptr, reinterpret_cast<void**>(&wvpData));

	const float LON_EVERY = float(std::numbers::pi) * 2.0f / float(VertexNum);
	//緯度分割1つ分の角度θd
	const float LAT_EVERY = float(std::numbers::pi) / float(VertexNum);
	{
		//緯度の方向に分割
		for (int latIndex = 0; latIndex < VertexNum; ++latIndex) {
			//θ
			float lat = -float(std::numbers::pi) / 2.0f + LAT_EVERY * latIndex;
			//経度の方向に分割しながら線を描く
			for (int lonIndex = 0; lonIndex < VertexNum; ++lonIndex) {
				uint32_t start = (latIndex * VertexNum + lonIndex) * 6;

				//texcoord専用のxy座標
				float u = float(lonIndex) / float(VertexNum);
				//下から0上が1になっていたので「1.0f-～」にして逆にする
				float v = 1.0f - float(latIndex) / float(VertexNum);

				float lon = lonIndex * LON_EVERY;
				//頂点にデータを入力する。基準点a
				//点間の距離
				float length = 1.0f / VertexNum;

#pragma region 三角形１枚目 
				//左上(点B)が原点
				//abc
				//資料通りだとここは点a(左下)
				vertexData[start].position.x = size_ * (cos(lat) * cos(lon));
				vertexData[start].position.y = size_ * (sin(lat));
				vertexData[start].position.z = size_ * (cos(lat) * sin(lon));
				vertexData[start].position.w = 1.0f;
				//分割分移動
				vertexData[start].texcoord.x = u;
				vertexData[start].texcoord.y = v + length;

				//点b(左上)
				vertexData[start + 1].position.x = size_ * (cos(lat + LAT_EVERY)) * cos(lon);
				vertexData[start + 1].position.y = size_ * (sin(lat + LAT_EVERY));
				vertexData[start + 1].position.z = size_ * (cos(lat + LAT_EVERY)) * sin(lon);
				vertexData[start + 1].position.w = 1.0f;
				vertexData[start + 1].texcoord.x = u;
				vertexData[start + 1].texcoord.y = v;


				//点c(右下)
				vertexData[start + 2].position.x = size_ * (cos(lat) * cos(lon + LON_EVERY));
				vertexData[start + 2].position.y = size_ * (sin(lat));
				vertexData[start + 2].position.z = size_ * (cos(lat) * sin(lon + LON_EVERY));
				vertexData[start + 2].position.w = 1.0f;
				vertexData[start + 2].texcoord.x = u + length;
				vertexData[start + 2].texcoord.y = v + length;

#pragma endregion

#pragma region 三角形２枚目
				//bcd
				//点d(右上)
				vertexData[start + 3].position.x = size_ * (cos(lat + LAT_EVERY) * cos(lon + LON_EVERY));
				vertexData[start + 3].position.y = size_ * (sin(lat + LAT_EVERY));
				vertexData[start + 3].position.z = size_ * (cos(lat + LAT_EVERY) * sin(lon + LON_EVERY));
				vertexData[start + 3].position.w = 1.0f;
				vertexData[start + 3].texcoord.x = u + length;
				vertexData[start + 3].texcoord.y = v;

				//点c(右下)
				vertexData[start + 4].position.x = size_ * (cos(lat) * cos(lon + LON_EVERY));
				vertexData[start + 4].position.y = size_ * (sin(lat));
				vertexData[start + 4].position.z = size_ * (cos(lat) * sin(lon + LON_EVERY));
				vertexData[start + 4].position.w = 1.0f;
				vertexData[start + 4].texcoord.x = u + length;
				vertexData[start + 4].texcoord.y = v + length;



				//点b(左上)
				vertexData[start + 5].position.x = size_ * (cos(lat + LAT_EVERY) * cos(lon));
				vertexData[start + 5].position.y = size_ * (sin(lat + LAT_EVERY));
				vertexData[start + 5].position.z = size_ * (cos(lat + LAT_EVERY) * sin(lon));
				vertexData[start + 5].position.w = 1.0f;
				vertexData[start + 5].texcoord.x = u;
				vertexData[start + 5].texcoord.y = v;
#pragma endregion
			}
		}
	}
	*wvpData = workdTransform_.matWorld;

	*MaterialData = color_;

	CommandCall();
}

void StateSphere::Release()
{
	CreateResources::Release(resource_.wvpResource);
	CreateResources::Release(resource_.Material);
	CreateResources::Release(resource_.Vertex);
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
	PSOProperty PSO = GraphicsPipeline::GetInstance()->GetPSO().sprite;


	commands.List->SetGraphicsRootSignature(PSO.rootSignature);
	commands.List->SetPipelineState(PSO.GraphicsPipelineState);//
	
	commands.List->IASetVertexBuffers(0, 1, &resource_.BufferView);

	//形状を設定。PSOに設定しているものとはまた別。同じものを設定すると考えておけば良い
	commands.List->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);


	//wvp用のCBufferの場所を設定
	commands.List->SetGraphicsRootConstantBufferView(1, resource_.wvpResource->GetGPUVirtualAddress());

	//マテリアルCBufferの場所を設定
	commands.List->SetGraphicsRootConstantBufferView(0, resource_.Material->GetGPUVirtualAddress());

	//
	commands.List->SetGraphicsRootDescriptorTable(2, tex_.SrvHandleGPU);


	//描画(DrawCall/ドローコール)。
	commands.List->DrawInstanced(VertexNum*VertexNum*6, 1, 0, 0);

}
