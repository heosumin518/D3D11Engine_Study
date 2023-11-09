#include "pch.h"
#include "Animator.h"
#include "TimeManager.h"

Animator::Animator()
	: Super(ComponentType::Animator)
{
}

Animator::~Animator()
{
}

void Animator::Init()
{

}

void Animator::Update()
{
	// 현재 실행하고 있는 애니메이션을 확인한다.
	shared_ptr<Animation> animation = GetCurrentAnimation();
	if (animation == nullptr)	// 실행 중인 애니메이션이 없다면 업데이트 하지 않는다.
		return;

	const Keyframe& keyframe = animation->GetKeyframe(_currentKeyframeIndex);

	float deltaTime = TIME->GetDeltaTime();
	_sumTime += deltaTime;

	// 누적 시간이 키프레임 시간을 초과했다면 다음 프레임으로 넘어간다.
	if (_sumTime >= keyframe.time)	
	{
		_currentKeyframeIndex++;
		int32 totalCount = animation->GetKeyframeCount();

		// 마지막 프레임까지 도달했을때
		if (_currentKeyframeIndex >= totalCount)
		{
			if (animation->IsLoop())	// 반복 재생이면 처음 프레임으로 돌아감
				_currentKeyframeIndex = 0;
			else
				_currentKeyframeIndex = totalCount - 1;		// 인덱스를 최대치로 둔다. -> 애니메이션 재생 끝
		}

		_sumTime = 0.f;
	}
}

shared_ptr<Animation> Animator::GetCurrentAnimation()
{
	return _currentAnimation;
}

const Keyframe& Animator::GetCurrentKeyframe()
{
	return _currentAnimation->GetKeyframe(_currentKeyframeIndex);
}

