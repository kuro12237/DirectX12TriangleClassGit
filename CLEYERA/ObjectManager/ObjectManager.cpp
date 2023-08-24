#include"ObjectManager.h"


ObjectManager::ObjectManager()
{
}

ObjectManager::~ObjectManager()
{
}

ModelData ObjectManager::LoadFile(const std::string& directoryPath, const std::string& filename)
{
	ModelData modelData = {};
	std::vector<Vector4> positions;
	std::vector<Vector3> normals;
	std::vector<Vector2> texcoords;
	std::string line;
	std::ifstream file(directoryPath + "/" + filename);
	assert(file.is_open());

	while (std::getline(file,line))
	{
		std::string identifier;
		std::istringstream s(line);
		s >> identifier;

		if (identifier=="v")
		{   //v頂点位置
			Vector4 position;
			s >> position.x >> position.y >> position.z;
			position.w = 1.0f;
			positions.push_back(position);
		}
		else if(identifier=="vt")
		{	//vt頂点テクスチャの座標
			Vector2 texcoord;
			s >> texcoord.x >> texcoord.y;
			texcoords.push_back(texcoord);
		}
		else if (identifier == "vn") 
		{   //vn頂点法線
			Vector3 normal;
			s >> normal.x >> normal.y >> normal.z;
			normals.push_back(normal);
		}
		else if (identifier == "f")
		{   //f::面
			for (int32_t faceVertex = 0; faceVertex < 3; ++faceVertex) {
				std::string vertexDefinition;
				s >> vertexDefinition;

				std::istringstream v(vertexDefinition);
				uint32_t elementIndices[3];
				for (int32_t element = 0; element < 3; ++element)
				{
					std::string index;
					std::getline(v, index, '/');
					elementIndices[element] = std::stoi(index);
				}
				//構築
				Vector4 position = positions[elementIndices[0] - 1];
				Vector2 texcoord = texcoords[elementIndices[1] - 1];
				Vector3 normal = normals[elementIndices[2] - 1];
				VertexData vertex = { position,texcoord,normal };
				modelData.vertices.push_back(vertex);
			}
		}
	}
	return modelData;
}

void ObjectManager::Initialize(Vector4 pos, float size, WorldTransform worldTransform, texResourceProperty tex,const std::string& directoryPath, const std::string& filename)
{
	
	modelData_=LoadFile(directoryPath,filename);
	//ここで止まる

	Resource_.Vertex = CreateResources::CreateBufferResource(sizeof(VertexData) * modelData_.vertices.size());
	Resource_.Material = CreateResources::CreateBufferResource(sizeof(Vector4));
	Resource_.wvpResource = CreateResources::CreateBufferResource(sizeof(TransformationMatrix));
	Resource_.Light = CreateResources::CreateBufferResource(sizeof(LightData));

	Resource_.BufferView.BufferLocation = Resource_.Vertex->GetGPUVirtualAddress();
	Resource_.BufferView.SizeInBytes = UINT(sizeof(VertexData) * modelData_.vertices.size());
	Resource_.BufferView.StrideInBytes = sizeof(VertexData);


	tex_ = tex;
	centerPos_ = pos;
	size_ = size;
	workdTransform_ = worldTransform;


}

void ObjectManager::Draw(Matrix4x4 m)
{
	VertexData* vertexData = nullptr;
	Vector4* materialData = nullptr;
	TransformationMatrix* matrixData = nullptr;
	LightData* lightData = nullptr;

	Resource_.Vertex->Map(0, nullptr, reinterpret_cast<void**>(&vertexData));
	Resource_.Material->Map(0, nullptr, reinterpret_cast<void**>(&materialData));
	Resource_.wvpResource->Map(0, nullptr, reinterpret_cast<void**>(&matrixData));
	Resource_.Light->Map(0, nullptr, reinterpret_cast<void**>(&lightData));
	std::memcpy(vertexData, modelData_.vertices.data(), sizeof(VertexData) * modelData_.vertices.size());

	matrixData->WVP = m;
	matrixData->world = MatrixTransform::Identity();

	*materialData = { 1,1,1,1 };

	Vector3 directionPos = { 0.0f,-0.0f,0.0f };

	lightData->direction = directionPos;
	lightData->color = { 0,0,0,1, };
	lightData->intensity = 0.0f;


	Commands commands = DirectXCommon::GetInstance()->GetCommands();
	PSOProperty PSO = GraphicsPipeline::GetInstance()->GetPSO().indirectLight;
	

	commands.List->SetGraphicsRootSignature(PSO.rootSignature);
	commands.List->SetPipelineState(PSO.GraphicsPipelineState);

	commands.List->IASetVertexBuffers(0, 1, &Resource_.BufferView);

	commands.List->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	commands.List->SetGraphicsRootConstantBufferView(0, Resource_.Material->GetGPUVirtualAddress());

	commands.List->SetGraphicsRootDescriptorTable(2, tex_.SrvHandleGPU);
	commands.List->SetGraphicsRootConstantBufferView(1, Resource_.wvpResource->GetGPUVirtualAddress());
	commands.List->SetGraphicsRootConstantBufferView(3, Resource_.Light->GetGPUVirtualAddress());

	commands.List->DrawInstanced(UINT(modelData_.vertices.size()),1,0,0);
}

void ObjectManager::Release()
{

	Resource_.Light->Release();
	Resource_.Material->Release();
	Resource_.Vertex->Release();
	Resource_.wvpResource->Release();


}


