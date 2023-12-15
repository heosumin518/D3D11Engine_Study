#pragma once

class Model;
struct NodeAnimation;
class Node
{
public:
	Node() {}
	~Node() {}

	void UpdateAnimation(float progressTime);
	shared_ptr<Node> FindNode(const string& name);

protected:
	string m_name;
	Matrix m_local;
	Matrix m_world;
	vector<Node> m_children;	// ���� ������ �̷�� �ڽ� ���
	vector<UINT> m_meshIndices;	// �ش� ���� ����Ǵ� �޽� �ε���
	shared_ptr<Node> m_parent;
	shared_ptr<NodeAnimation> m_nodeAnimation;	// ��尡 ����� NodeAnimation�� �ִ� �ּ�
};

