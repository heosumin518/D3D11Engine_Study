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

// 특정 노드의 시간에서의 Transform
struct NodeAnimation
{
	string nodeName;			// 애니메이션이 적용될 계층 구조에서의 노드 이름
	vector<AnimationKey> animationKeys;
	// aiNodeAnim으로 AnimationKey를 생성
	void Create(aiNodeAnim* nodeAnimation, double tickPerSecond);
	// 시간에 따른 보간된 Key값을 계산하는 함수
	void Evaluate(float time, Vector3& position, Quaternion& rotation, Vector3& scaling);
};

// 모든 노드의 각 시간에서의 Transform
struct Animation
{
	float duration = 0.f;	// 전체 애니메이션 길이
	vector<NodeAnimation> nodeAnimations;
};

