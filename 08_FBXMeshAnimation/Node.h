#pragma once

class Model;
class Mesh;
class NodeAnimation;
class ModelLoader;

class Node : public enable_shared_from_this<Node>
{
public:
	void Update(float deltaTime);

	void SetParentNode(shared_ptr<Node> parent) { m_parent = parent; m_parent->SetChildNode(shared_from_this()); }
	void SetChildNode(shared_ptr<Node> child) { m_children.push_back(child); }

	void Evaluate(float currentTime, Vector3& position, Quaternion& rotation, Vector3& scale);

public:
	friend ModelLoader;
	friend Node;
	friend Mesh;

private:
	string m_name;
	Matrix m_local;
	Matrix m_world;
	shared_ptr<Node> m_parent;
	vector<shared_ptr<Node>> m_children;

	shared_ptr<Mesh> m_mesh;
	shared_ptr<NodeAnimation> m_nodeAnim;
};

