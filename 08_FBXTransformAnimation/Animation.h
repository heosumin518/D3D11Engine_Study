#pragma once

struct KeyFrameData
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

	vector<KeyFrameData> keyFrame;

	void Update(float deltaTime);
	void Evaluate(float progressTime, Vector3& position, Quaternion& rotation, Vector3& scaling);
};

struct Animation
{
	string name;
	uint32 frameCount;
	float frameRate;
	float duration;
	vector<shared_ptr<NodeAnimation>> nodeAnimations;

	void Update(float deltaTime);
};

