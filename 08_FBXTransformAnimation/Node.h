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
	vector<Node> m_children;	// 계층 구조를 이루는 자식 노드
	vector<UINT> m_meshIndices;	// 해당 노드와 연결되는 메시 인덱스
	shared_ptr<Node> m_parent;
	shared_ptr<NodeAnimation> m_nodeAnimation;	// 노드가 사용할 NodeAnimation이 있는 주소
};

