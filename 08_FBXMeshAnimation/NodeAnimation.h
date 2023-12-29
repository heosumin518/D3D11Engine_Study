#pragma once

class Node;
class ModelLoader;

struct KeyFrameData
{
	float time;
	Vector3 scale;
	Quaternion rotation;
	Vector3 translation;
};

class NodeAnimation
{
public:
	friend ModelLoader;
	friend Node;

private:
	string m_name;
	unsigned int m_frameCount;
	float m_frameRate;
	vector<KeyFrameData> m_keyFrameBox;
};

