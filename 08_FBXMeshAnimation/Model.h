#pragma once
#include "Mesh.h"
#include "Material.h"
#include "ModelLoader.h"

/// Bone, Mesh, Material을 가지고 있다.

class Model
{
public:
	Model();
	~Model();

public:
	void Render(ComPtr<ID3D11DeviceContext> deviceContext);

public:
	vector<shared_ptr<Mesh>> GetMeshes() { return m_meshes; }
	vector<shared_ptr<Material>> GetMaterials() { return m_materials; }

private:
	friend ModelLoader;

	vector<shared_ptr<Material>> m_materials;
	vector<shared_ptr<Mesh>> m_meshes;

	shared_ptr<Node> m_rootNode;
};

