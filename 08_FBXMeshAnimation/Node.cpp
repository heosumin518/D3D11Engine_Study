#include "pch.h"
#include "Node.h"
#include "Model.h"
#include "Mesh.h"
#include "ModelLoader.h"
#include "NodeAnimation.h"

void Node::Update(float deltaTime)
{
	// 애니메이션이 있는 경우 애니메이션 보간 및 로컬 트랜스폼 업데이트
	if (m_nodeAnim != nullptr)
	{
		static float currentTime = 0.f;
		currentTime += DT;

		if (currentTime > m_nodeAnim->m_frameCount)
			currentTime -= m_nodeAnim->m_frameCount;

		Vector3 position, scale;
		Quaternion rotation;

		// 애니메이션 보간
		Evaluate(currentTime, position, rotation, scale);

		m_local = Matrix::CreateScale(scale) *
			Matrix::CreateFromQuaternion(rotation) *
			Matrix::CreateTranslation(position);
	}

	// 부모가 있으면 해당 부모의 트랜스폼 곱하기
	if (m_parent != nullptr)
	{
		m_world = m_local * m_parent->m_world;
	}
	else
	{
		m_world = m_local;
	}

	for (auto& child : m_children)
	{
		if (child != nullptr)
			child->Update(deltaTime);
	}
}

void Node::Evaluate(float currentTime, Vector3& position, Quaternion& rotation, Vector3& scale)
{
	int frameIndex = 0;

	// 현재 시간을 가장 가까운 키프레임으로 보간
	for (UINT i = 0; i < m_nodeAnim->m_frameCount - 1; i++)
	{
		if (currentTime < m_nodeAnim->m_keyFrameBox[frameIndex + 1].time)
			break;

		frameIndex = i;
	}

	const KeyFrameData& currentKey = m_nodeAnim->m_keyFrameBox[frameIndex];
	const KeyFrameData& nextKey = m_nodeAnim->m_keyFrameBox[frameIndex + 1];

	// 현재 키프레임과 다음 키프레임의 시간 차이 계산
	float deltaTime = nextKey.time - currentKey.time;

	// 만약 다음키프레임과 현재 키프레임의 시간 차이가 없다면 1로 보정 ( 계산 오류로 애니메이션 보간 정보가 날아가는 경우 방지하기 위해 )
	if (deltaTime == 0)
		deltaTime = 1;

	// 현재 시간이 현재 키프레임과 다음 키프레임 사이의 비율 계산. 이 값은 선형보간을 계산하기 위한 가중치로서 쓰인다.
	float factor = 0;
	if (currentKey.time < currentTime)
		factor = (currentTime - currentKey.time) / deltaTime;

	// 선형 보간
	position = Vector3::Lerp(currentKey.translation, nextKey.translation, factor);
	rotation = Quaternion::Slerp(currentKey.rotation, nextKey.rotation, factor);
	scale = Vector3::Lerp(currentKey.scale, nextKey.scale, factor);
}
