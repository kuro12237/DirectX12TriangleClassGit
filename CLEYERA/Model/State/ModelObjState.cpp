#include "ModelObjState.h"

void ModelObjState::Initialize(Model* state)
{
	ModelData_ = LoadFile(state,state->GetObjDirectoryPath());
	resource_.Vertex = CreateResources::CreateBufferResource(sizeof(VertexData) * ModelData_.vertices.size());
	resource_.Material = CreateResources::CreateBufferResource(sizeof(Material));
	resource_.wvpResource = CreateResources::CreateBufferResource(sizeof(TransformationMatrix));
	resource_.BufferView = CreateResources::VertexCreateBufferView(sizeof(VertexData) * ModelData_.vertices.size(), resource_.Vertex,int( ModelData_.vertices.size()));

}

void ModelObjState::Draw(Model* state, WorldTransform worldTransform, ViewProjection viewprojection)
{
	VertexData* vertexData = nullptr;
	Material* materialData = nullptr;

	resource_.Vertex->Map(0, nullptr, reinterpret_cast<void**>(&vertexData));
	resource_.Material->Map(0, nullptr, reinterpret_cast<void**>(&materialData));

	memcpy(vertexData, ModelData_.vertices.data(), sizeof(VertexData) * ModelData_.vertices.size());

	materialData->color = state->GetColor();
	materialData->uvTransform = MatrixTransform::AffineMatrix(state->GetuvScale(), state->GetuvRotate(), state->GetuvTranslate());


	worldTransform.TransfarMatrix(resource_.wvpResource, viewprojection);
	Commands commands = DirectXCommon::GetInstance()->GetCommands();
	SPSOProperty PSO = GraphicsPipelineManager::GetInstance()->GetPso().Sprite;

	commands.m_pList->SetGraphicsRootSignature(PSO.rootSignature.Get());
	commands.m_pList->SetPipelineState(PSO.GraphicsPipelineState.Get());

	commands.m_pList->IASetVertexBuffers(0, 1, &resource_.BufferView);

	commands.m_pList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	commands.m_pList->SetGraphicsRootConstantBufferView(0, resource_.Material->GetGPUVirtualAddress());

	TextureManager::texCommand(state->GetTexHandle());
	commands.m_pList->SetGraphicsRootConstantBufferView(1, resource_.wvpResource->GetGPUVirtualAddress());
	
	commands.m_pList->DrawInstanced(UINT(ModelData_.vertices.size()), 1, 0, 0);

}


SModelData ModelObjState::LoadFile(Model *state,const string& directoryPath)
{
	SModelData modelData = {};
	vector<Vector4> positions;
	vector<Vector3> normals;
	vector<Vector2> texcoords;
	string line;
	ifstream file("Resources/" + directoryPath + "/" + directoryPath + ".obj");
	assert(file.is_open());

	while (getline(file, line))
	{
		string identifier;
		istringstream s(line);
		s >> identifier;

		if (identifier == "v")
		{   //v頂点位置
			Vector4 position = {};
			s >> position.x >> position.y >> position.z;

			position.z *= -1.0f;
			position.w = 1.0f;
			positions.push_back(position);
		}
		else if (identifier == "vt")
		{	//vt頂点テクスチャの座標
			Vector2 texcoord = {};
			s >> texcoord.x >> texcoord.y;
			texcoord.y *= -1.0f;

			texcoords.push_back(texcoord);
		}
		else if (identifier == "vn")
		{   //vn頂点法線
			Vector3 normal = {};

			s >> normal.x >> normal.y >> normal.z;
			normal.z *= -1.0f;
			normals.push_back(normal);
		}
		else if (identifier == "f") {
			VertexData triangle[3] = {};
			for (int32_t faceVertex = 0; faceVertex < 3; ++faceVertex) {
				string vertexDefinition = {};
				s >> vertexDefinition;
				//分解してIndexをGet
				istringstream v(vertexDefinition);
				uint32_t elementIndices[3] = {};
				for (int32_t element = 0; element < 3; ++element) {
					string index;
					getline(v, index, '/');
					elementIndices[element] = stoi(index);
				}

				Vector4 position = positions[elementIndices[0] - 1];
				Vector2 texcoord = texcoords[elementIndices[1] - 1];
				Vector3 normal = normals[elementIndices[2] - 1];
				triangle[faceVertex] = { position,texcoord,normal };
			}
			//頂点を逆順で登録することで、回り順を逆にする
			modelData.vertices.push_back(triangle[2]);
			modelData.vertices.push_back(triangle[1]);
			modelData.vertices.push_back(triangle[0]);
		}
		else if (identifier == "mtllib") {
			//tex
			string materialFilename;
			s >> materialFilename;
			modelData.material = LoadMaterialTemplateFile(directoryPath, materialFilename);
		}

	}
	uint32_t texHandle = TextureManager::LoadTexture(modelData.material.textureFilePath);

	state->SetTexHandle(texHandle);
	
	return modelData;
}

MaterialData ModelObjState::LoadMaterialTemplateFile(const string& directoryPath, const string& filename)
{
	MaterialData materialData;
	string line;
	ifstream file("Resources/" + directoryPath + "/" + filename);
	assert(file.is_open());
	while (getline(file, line))
	{
		string identifier;
		istringstream s(line);
		s >> identifier;

		if (identifier == "map_Kd")
		{
			string texfilename;
			s >> texfilename;
			materialData.textureFilePath = "Resources/" + directoryPath + "/" + texfilename;

		}
	}

	return materialData;
}
