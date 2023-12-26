#pragma once

struct AnimationKey
{
	float time;
	Vector3 position;
	Quaternion rotation;
	Vector3 scale;
};

class Animation;

class NodeAnimation
{
public:
	void Evaluate(float progressTime, Vector3& position, Quaternion& rotation, Vector3& scale);

public:
	friend Animation;
	friend ModelLoader;

private:
	string m_name;
	vector<AnimationKey> m_animationKeys;

	int m_frameIndex = 0;
	float m_duration = 0;
};

