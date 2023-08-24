#pragma once
#include<list>
#include<vector>
#include"CreateResources/CreateResources.h"
#include<fstream>
#include<sstream>
#include"WorldTransform.h"
#include"GraphicsPipeline.h"
#include"TexManager.h"

struct ModelData
{
	std::vector<VertexData> vertices;
};

class ObjectManager
{
public:
	ObjectManager();
	~ObjectManager();



     ModelData LoadFile(const std::string& directoryPath, const std::string& filename);

	 void Initialize(Vector4 pos, float size, WorldTransform worldTransform, texResourceProperty tex, const std::string& directoryPath, const std::string& filename);

	 void Draw(Matrix4x4 m);

	 void Release();
private:

	ModelData modelData_ = {};
	ResourcePeroperty Resource_ = {};



	texResourceProperty tex_;
	WorldTransform workdTransform_ = {};
	Vector4 color_ = { 1,1,1,1 };
	Vector4 centerPos_ = { 0,0,0,0 };
	float size_ = 0;
};