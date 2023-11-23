#pragma once
#include <filesystem>
// FBX 파일로부터 정보를 로드하여 Model 객체에 넘겨주는 역할을 하는 클래스

class Model;
class Mesh;
class Material;

class ModelLoader
{
public:
	ModelLoader(ComPtr<ID3D11Device> m_device);
	~ModelLoader();
public:
	shared_ptr<Model> LoadModelFile(const string& file);
	void CreateNode(aiNode* node);
	void CreateMesh(aiNode* node);
	void CreateMaterial();

private:
	ComPtr<ID3D11Device> m_device;

private:
	shared_ptr<Assimp::Importer> m_importer;
	const aiScene* m_scene = nullptr;

private:
	//vector<shared_ptr<Bone>> m_bones;
	vector<shared_ptr<Mesh>> m_meshes;
	vector<shared_ptr<Material>> m_materials;
};

