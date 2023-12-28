#pragma once

class ModelLoader;

class Node
{
public:
	void Update();

public:
	friend ModelLoader;

private:
	string m_name;
	Matrix m_local;
	Matrix m_world;
	shared_ptr<Node> m_parent;
};

