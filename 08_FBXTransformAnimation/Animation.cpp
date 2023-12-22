#include "pch.h"
#include "Animation.h"
#include "Node.h"
#include "Model.h"
#include "ModelLoader.h"

int Animation::fps = 30;

void Animation::Update(float deltaTime)
{
	m_duration += deltaTime;

	// 다음 키프레임 인덱스 계산.
	m_nextKey = (m_curKey + 1) % m_animationKeys.size();

	// 만약 현재 진행된 애니메이션 시간이 다음 키프레임의 시간을 초과한다면,
	if (m_duration > m_animationKeys[m_nextKey].time / fps)
	{
		// 현재 키프레임을 다음 키프레임으로 업데이트
		++m_curKey %= m_animationKeys.size();

		// 다음 키프레임도 업데이트
		++m_nextKey %= m_animationKeys.size();

		// 만약 다음 키프레임이 처음 키프레임이라면, 마지막 키프레임의 시간만큼 현재 진행된 애니메이션 시간을 줄인다.
		if (m_nextKey == 0)
			m_duration -= m_animationKeys.back().time / fps;
	}

	Vector3 scale, position;
	Quaternion rotation;

	// 보간
	Evaluate(scale, rotation, position);
}

void Animation::Evaluate(Vector3& scale, Quaternion& rotation, Vector3& position)
{
	// 현재 키프레임과 다음 키프레임 사이의 시간 간격 계산
	float interval = (m_animationKeys[m_nextKey].time - m_animationKeys[m_curKey].time) / fps;	// fps 로 나누어서 초당 프레임 간격으로 변환

	// 현재 키프레임에서 얼마나 시간이 흘렀는지
	float curKeyframeProgress = m_duration - m_animationKeys[m_curKey].time / fps;
	
	// 현재 시간이 현재 키프레임과 다음 키프레임 간의 시간 간격에서 차지하는 비율 ( 0 ~ 1 ) // 현재 애니메이션의 진행도를 나타내기도함.
	float ratio = curKeyframeProgress / interval;

	// 선형 보간
	scale = Vector3::Lerp(m_animationKeys[m_curKey].scale, m_animationKeys[m_nextKey].scale, ratio);
	position = Vector3::Lerp(m_animationKeys[m_curKey].position, m_animationKeys[m_nextKey].position, ratio);
	rotation = Quaternion::Slerp(m_animationKeys[m_curKey].rotation, m_animationKeys[m_nextKey].rotation, ratio);

	m_node->SetLocalTransform(Matrix::CreateScale(scale) * Matrix::CreateFromQuaternion(rotation) * Matrix::CreateTranslation(position));
}
