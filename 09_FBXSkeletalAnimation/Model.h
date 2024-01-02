#pragma once
#include "Mesh.h"
#include "Material.h"
#include "ModelLoader.h"

class Model
{
public:
	Model();
	~Model();

public:
	void Update(float deltaTime);
	void Render(ComPtr<ID3D11DeviceContext> deviceContext);

public:
	vector<shared_ptr<Mesh>> GetMeshes() { return m_meshes; }
	vector<shared_ptr<Material>> GetMaterials() { return m_materials; }

private:
	friend ModelLoader;

	vector<shared_ptr<Material>> m_materials;
	vector<shared_ptr<Mesh>> m_meshes;
	vector<shared_ptr<Node>> m_nodes;
	shared_ptr<Node> m_rootNode;
	shared_ptr<Animation> m_animation;
};

