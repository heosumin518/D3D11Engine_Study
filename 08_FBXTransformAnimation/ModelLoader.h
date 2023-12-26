#pragma once

class Node;
class Model;
class Mesh;
class Material;
class Animation;
class ModelLoader
{
public:
	ModelLoader(ComPtr<ID3D11Device> device);
	~ModelLoader() {}

public:
	shared_ptr<Model> LoadModelFile(const string& file);
	void CreateMaterial();
	void CreateAnimation(aiAnimation* srcAnim);
	shared_ptr<Node> CreateNode(shared_ptr<Model> model, aiNode* srcNode, shared_ptr<Node> parent);
	void CreateMesh(shared_ptr<Model> model, aiNode* srcNode, shared_ptr<Node> node);

private:
	ComPtr<ID3D11Device> m_device;

private:
	shared_ptr<Assimp::Importer> m_importer;
	const aiScene* m_scene = nullptr;

private:
	shared_ptr<Node> m_rootNode;
	vector<shared_ptr<Mesh>> m_meshes;
	vector<shared_ptr<Material>> m_materials;
	shared_ptr<Animation> m_animation;
};

