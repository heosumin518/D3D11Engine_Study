#pragma once

class ModelLoader;
class Node;
class Mesh;
class Material;
class Animation;

class Model : enable_shared_from_this<Model>
{
public:
	Model();
	~Model();

	void Update(float deltaTime);
	//void ReadFile(ComPtr<ID3D11Device> device, const char* filePath);
	shared_ptr<Material> GetMaterialByIndex(UINT index) { return m_materials[index]; }

public:
	friend ModelLoader;
	friend Node;

private:
	const aiScene* m_scene;

	shared_ptr<Node> m_rootNode;
	vector<shared_ptr<Mesh>> m_meshes;
	vector<shared_ptr<Material>> m_materials;
	shared_ptr<Animation> m_animation;
};

