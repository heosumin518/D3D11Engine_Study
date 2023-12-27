#include "pch.h"
#include "Node.h"
#include "Mesh.h"
#include "Model.h"
#include "Animation.h"
#include "NodeAnimation.h"
#include "ModelLoader.h"

Node::Node()
{
}

Node::~Node()
{
}

void Node::Update(float deltaTime)
{
	// 애니메이션이 있다면 보간하고 Local Transform 업데이트
	if (m_pNodeAnimation != nullptr)
	{
		Vector3 position, scale;
		Quaternion rotation;

		m_pNodeAnimation->Evaluate(deltaTime, position, rotation, scale);

		m_local = Matrix::CreateScale(scale) * \
			Matrix::CreateFromQuaternion(rotation) * \
			Matrix::CreateTranslation(position);
	}

	// 부모가 있다면 부모의 월드 트랜스폼을 곱해준다.
	if (m_parent != nullptr)
		m_world = m_local * m_parent->m_world;
	else
		m_world = m_local;

	for (auto& child : m_children)
		child->Update(deltaTime);
}

