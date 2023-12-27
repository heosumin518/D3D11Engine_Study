#include "pch.h"
#include "Animation.h"
#include "Node.h"
#include "NodeAnimation.h"
#include "ModelLoader.h"

// 애니메이션 보간
void NodeAnimation::Evaluate(float progressTime, Vector3& position, Quaternion& rotation, Vector3& scale)
{
	// 애니메이션 진행시간 증가
	m_duration += progressTime;

	// 현재 프레임인덱스값이 마지막 키프레임에 도달했을 경우
	if (m_frameIndex + 1 >= m_animationKeys.size())
	{
		m_frameIndex = 0;
		m_duration = 0;
	}

	const AnimationKey& currentKey = m_animationKeys[m_frameIndex];
	const AnimationKey& nextKey = m_animationKeys[m_frameIndex + 1];

	// 현재 애니메이션의 진행시간이 다음키프레임의 시간을 초과했다면 키프레임 증가
	// 즉, 현재 프레임이 다음키프레임에 도달했는지 확인
	if (m_duration > nextKey.time)
		m_frameIndex++;

	// 현재 시간이 현재 키프레임과 다음 키프레임 사이에서 어느 정도에 위치해있는지 계산. 이 값은 선형보간을 계산하기 위한 가중치로서 쓰인다.
	float factor = (m_duration - currentKey.time) / (nextKey.time - currentKey.time);

	position = Vector3::Lerp(currentKey.position, nextKey.position, factor);
	rotation = Quaternion::Slerp(currentKey.rotation, nextKey.rotation, factor);
	scale = Vector3::Lerp(currentKey.scale, currentKey.scale, factor);
}
