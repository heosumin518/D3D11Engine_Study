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
	aiString name;
	vector<AnimationKey> animationKeys;
};

struct Animation
{
	string name;
	uint32 frameCount;
	float frameRate;
	float duration;
	vector<shared_ptr<NodeAnimation>> nodeAnimations;
};

