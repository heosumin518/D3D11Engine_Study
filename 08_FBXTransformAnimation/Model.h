#pragma once

class Node;
class Mesh;
class Material;
class Animation;

class Model
{
public:
	Model();
	~Model();

	void Update(float deltaTime);
	bool ReadFile(ComPtr<ID3D11Device> device, const char* filePath);
	shared_ptr<Material> GetMaterialByIndex(UINT index);

private:
	const aiScene* m_scene;

	shared_ptr<Node> m_rootNode;
	vector<shared_ptr<Mesh>> m_meshes;
	vector<shared_ptr<Material>> m_materials;
	shared_ptr<Animation> m_animation;
};

