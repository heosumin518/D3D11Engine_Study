#pragma once

struct aiNodeAnim;

// Transform
struct AnimationKey
{
	float time = 0.f;
	Vector3 position;
	Quaternion rotation;
	Vector3 scaling;
};

// Ư�� ����� �ð������� Transform
struct NodeAnimation
{
	string nodeName;			// �ִϸ��̼��� ����� ���� ���������� ��� �̸�
	vector<AnimationKey> animationKeys;
	// aiNodeAnim���� AnimationKey�� ����
	void Create(aiNodeAnim* nodeAnimation, double tickPerSecond);
	// �ð��� ���� ������ Key���� ����ϴ� �Լ�
	void Evaluate(float time, Vector3& position, Quaternion& rotation, Vector3& scaling);
};

// ��� ����� �� �ð������� Transform
struct Animation
{
	float duration = 0.f;	// ��ü �ִϸ��̼� ����
	vector<NodeAnimation> nodeAnimations;
};

