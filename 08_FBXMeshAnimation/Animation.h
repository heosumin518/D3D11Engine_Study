#pragma once
class ModelLoader;

class NodeAnimation;

class Animation
{
public:

public:
	friend ModelLoader;

private:
	string m_name;
	unsigned int m_frameCount;
	float m_frameRate;
	vector<shared_ptr<NodeAnimation>> m_nodeAnimations;
};

