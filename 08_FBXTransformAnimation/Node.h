#pragma once
#include "ModelLoader.h"


class Node : public std::enable_shared_from_this<Node>
{
public:
	Node();
	~Node();

public:
	void Init(ComPtr<ID3D11Device> device, ComPtr<ID3D11Buffer> nodebuffer);
	void Update(ComPtr<ID3D11DeviceContext> deviceContext, float deltaTime);

	friend ModelLoader;

public:
	void SetParentNode(shared_ptr<Node> parent) { m_parent = parent; m_parent->SetChildNode(this->shared_from_this()); }
	void SetChildNode(shared_ptr<Node> child) { m_children.push_back(child); }
	void SetMesh(shared_ptr<Mesh> mesh) { m_mesh = mesh; }

	void SetAnimation(shared_ptr<NodeAnimation> nodeAnimation) { m_nodeAnimation = nodeAnimation; }

	string GetName() { return m_name; }

public:
	void InterpolateAnimationData(float currentTime, Vector3& outPosition, Vector3& outScaling, Quaternion& outRotation);

private:
	string m_name;
	int32 m_index;
	int32 m_parentIndex;

	Matrix m_localTransform;
	Matrix m_worldTransform;

	vector<shared_ptr<Mesh>> m_meshes;

	shared_ptr<Mesh> m_mesh;

	shared_ptr<NodeAnimation> m_nodeAnimation;

	shared_ptr<Node> m_parent;
	vector<shared_ptr<Node>> m_children;

	ComPtr<ID3D11Buffer> m_nodeBuffer;
};

