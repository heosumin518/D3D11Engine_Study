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
	void Init(ComPtr<ID3D11Device> device, ComPtr<ID3D11Buffer> modelData);

	void Update(ComPtr<ID3D11DeviceContext> deviceContext, float deltaTime);
	//void Render(ComPtr<ID3D11DeviceContext> deviceContext);

public:
	void SetHeadNode(shared_ptr<Node> node) { m_rootNode = node; }
	void SetNode(shared_ptr<Node> node) { m_nodes.push_back(node); }
	vector<shared_ptr<Node>>& GetNode() { return m_nodes; }

	vector<shared_ptr<Material>> GetMaterials() { return m_materials; }

	void SetAnimation(shared_ptr<Animation> animation) { m_animation = animation; }
	shared_ptr<Animation> GetAnimation() { return m_animation; }

private:
	friend ModelLoader;

	shared_ptr<Node> m_rootNode;
	shared_ptr<Animation> m_animation;
	vector<shared_ptr<Node>> m_nodes;
	vector<shared_ptr<Material>> m_materials;
	ComPtr<ID3D11Buffer> m_CBNodeData = nullptr;
};

