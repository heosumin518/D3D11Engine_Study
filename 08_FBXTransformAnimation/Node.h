#pragma once
#include "ModelLoader.h"


class Node
{
public:
	Node();
	~Node();

public:
	void Update(); 

	friend ModelLoader;

private:
	string m_name;
	int32 m_index;
	int32 m_parentIndex;
	shared_ptr<Node> parent;

	Matrix m_transform;
	vector<shared_ptr<Node>> m_children;
};

