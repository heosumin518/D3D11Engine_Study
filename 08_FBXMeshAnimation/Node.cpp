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
		currentTime += deltaTime * 3;

		if (currentTime > m_nodeAnim->m_frameCount)
			currentTime -= m_nodeAnim->m_frameCount;

	}

	for (auto& child : m_children)
	{
		if (child != nullptr)
			child->Update(deltaTime);
	}
}