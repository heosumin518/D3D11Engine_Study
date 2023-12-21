#include "pch.h"
#include "Animation.h"

void NodeAnimation::Evaluate(float progressTime, Vector3& position, Quaternion& rotation, Vector3& scaling)
{
	assert(animationKeys.size() > 0);

	if (frameIndex + 1 >= animationKeys.size())
	{
		frameIndex = 0;
		duration = 0;
	}

	const AnimationKey& currentKey = animationKeys[frameIndex];
	const AnimationKey& nextKey = animationKeys[frameIndex + 1];

	if (progressTime > nextKey.time)
		frameIndex++;

	float time = (progressTime - currentKey.time) / (nextKey.time - currentKey.time);

	position = Vector3::Lerp(currentKey.translation, nextKey.translation, time);
	rotation = Quaternion::Slerp(currentKey.rotation, nextKey.rotation, time);
	scaling = Vector3::Lerp(currentKey.scale, nextKey.scale, time);
}
