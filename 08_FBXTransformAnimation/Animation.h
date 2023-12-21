#pragma once

struct AnimationKey
{
	float time;
	Vector3 scale;
	Quaternion rotation;
	Vector3 translation;
};

struct NodeAnimation
{
	string name;
	uint32 frameCount;
	float frameRate;
	float duration;
	vector<AnimationKey> animationKeys;

	uint32 frameIndex = 0;

	void Evaluate(float progressTime, Vector3& position, Quaternion& rotation, Vector3& scaling);
};

struct Animation
{
	string name;
	uint32 frameCount;
	float frameRate;
	float duration;
	vector<shared_ptr<NodeAnimation>> nodeAnimations;
};

