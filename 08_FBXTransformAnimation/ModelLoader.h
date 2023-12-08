#pragma once
#include <filesystem>
// FBX ���Ϸκ��� ������ �ε��Ͽ� Model ��ü�� �Ѱ��ִ� ������ �ϴ� Ŭ����

class Node;
class Model;
class Mesh;
class Material;
struct Animation;
struct NodeAnimation;

class ModelLoader
{
public:
	ModelLoader(ComPtr<ID3D11Device> m_device);
	~ModelLoader();
public:
	shared_ptr<Model> LoadModelFile(const string& file);
	void ReadAnimationData(aiAnimation* animation);
	shared_ptr<NodeAnimation> ParseAnimationNode(shared_ptr<Animation> animation, aiNodeAnim* srcNode);
	void CreateNode(aiNode* srcNode, shared_ptr<Node> parentNode);
	void CreateMesh(aiNode* node, shared_ptr<Node> connectNode);
	void CreateMaterial();

private:
	ComPtr<ID3D11Device> m_device;

private:
	shared_ptr<Assimp::Importer> m_importer;
	const aiScene* m_scene = nullptr;

private:
	shared_ptr<Model> m_model;

	//vector<shared_ptr<Node>> m_bones;
	//vector<shared_ptr<Mesh>> m_meshes;
	vector<shared_ptr<Material>> m_materials;
	vector<shared_ptr<Animation>> m_animations;
};

