#pragma once
#include <filesystem>
// FBX ���Ϸκ��� ������ �ε��Ͽ� Model ��ü�� �Ѱ��ִ� ������ �ϴ� Ŭ����

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

