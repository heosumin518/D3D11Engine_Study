#pragma once

class ModelLoader;
class Model;
class Animation;
class NodeAnimation;

class Node : public enable_shared_from_this<Node>
{
public:
	Node();
	~Node();

public:
	void Update(float deltaTime);

public:
	friend ModelLoader;

private:
	string m_name;

	shared_ptr<Node> m_parent;
	shared_ptr<NodeAnimation> m_pNodeAnimation;

	vector<UINT> m_meshIndices;
	vector<shared_ptr<Node>> m_children;

	Matrix m_local = Matrix::Identity;
	Matrix m_world = Matrix::Identity;
};

