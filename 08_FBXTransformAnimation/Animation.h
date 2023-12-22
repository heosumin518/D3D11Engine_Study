#pragma once

struct AnimationKey
{
	float time = 0.f;
	Vector3 position = {};
	Vector3 scale = { 1, 1, 1};
	Quaternion rotation = {};
};

class ModelLoader;
class Model;
class Node;
class Animation
{
public:
	void Update(float deltaTime);
	void Evaluate(Vector3& scale, Quaternion& rotation, Vector3& position);

private:
	friend ModelLoader;

	vector<AnimationKey> m_animationKeys;
	shared_ptr<Node> m_node;
	shared_ptr<Model> m_owner;

	string m_name;
	uint32 m_curKey = 0;
	uint32 m_nextKey = 0;
	float m_duration = 0.f;		// 애니메이션 경과 시간

	static int fps;
};

