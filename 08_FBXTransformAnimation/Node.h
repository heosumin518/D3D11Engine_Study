#pragma once
#include "ModelLoader.h"
#include "Struct.h"

class Node : public enable_shared_from_this<Node>
{
public:
	Node();
	~Node();

public:
	void Init(ComPtr<ID3D11Buffer> modelBuffer);
	void Update(float deltaTime);
	void Render(ComPtr<ID3D11DeviceContext> deviceContext);

	string GetName() { return m_name; }
	void SetNodeAnimation(shared_ptr<NodeAnimation> nodeAnimation) { m_nodeAnimation = nodeAnimation; }

	void SetParentNode(shared_ptr<Node> parent) { m_parent = parent; m_parent->SetChildNode(shared_from_this()); }
	void SetChildNode(shared_ptr<Node> child) { m_children.push_back(child); }

	void SetMesh(shared_ptr<Mesh> mesh) { m_mesh = mesh; }

	void SetWorldTransform(Matrix& transform) { m_matWorld = transform; }

	friend ModelLoader;

private:
	string m_name;
	int32 m_index;
	int32 m_parentIndex;

	shared_ptr<Mesh> m_mesh;

	Matrix m_matLocal;
	Matrix m_matWorld;

	shared_ptr<Node> m_parent;
	vector<shared_ptr<Node>> m_children;

	CB_ModelTransform m_CBNodeTransform;
	ComPtr<ID3D11Buffer> m_nodeBuffer;	// TODO 231221 노드버퍼 정보를 넘겨주어야 함

private:
	shared_ptr<NodeAnimation> m_nodeAnimation;
};

