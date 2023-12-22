#pragma once
#include "Node.h"
#include "Mesh.h"
#include "Material.h"
#include "Animation.h"
#include "ModelLoader.h"

/// Bone, Mesh, Material을 가지고 있다.

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
	Matrix GetTransform() { return m_transform; }
private:
	friend ModelLoader;

	shared_ptr<Node> m_root;	// 루트 노드
	vector<shared_ptr<Node>> m_nodes;	// 계층구조는 parentIndex로 구분.
	vector<shared_ptr<Mesh>> m_meshes;
	vector<shared_ptr<Material>> m_materials;
	vector<shared_ptr<Animation>> m_animations;

	Vector3 m_position = { 0, 0, 0 };
	Vector3 m_scale = { 1, 1, 1 };
	Vector3 m_rotation = { 0, 0, 0 };
	Matrix m_transform = {};

};

