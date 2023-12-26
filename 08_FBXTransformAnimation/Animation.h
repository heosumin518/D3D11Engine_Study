#pragma once


class ModelLoader;
class NodeAnimation;

class Animation
{
public:
	Animation() {}
	~Animation() {}

public:
	friend ModelLoader;

private:
	string m_name;
	vector<shared_ptr<NodeAnimation>> m_nodeAnimations;
	float m_duration;
	float m_ticksPerSecond;
};

