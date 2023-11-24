#pragma once


class Node
{
public:
	Node();
	~Node();

public:
	void Update(); 

private:
		string m_name;
		int32 m_index = -1;
		int32 m_parent = -1;
		Matrix m_transform;
};

