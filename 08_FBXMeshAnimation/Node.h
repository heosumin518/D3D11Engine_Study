#pragma once

class ModelLoader;

class Node : public enable_shared_from_this<Node>
{
public:
	void Update();

	void SetParentNode(shared_ptr<Node> parent) { m_parent = parent; m_parent->SetChildNode(shared_from_this()); }
	void SetChildNode(shared_ptr<Node> child) { m_children.push_back(child); }

public:
	friend ModelLoader;

private:
	string m_name;
	Matrix m_local;
	Matrix m_world;
	shared_ptr<Node> m_parent;
	vector<shared_ptr<Node>> m_children;
};

