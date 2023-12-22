#include "pch.h"
#include "Node.h"

#include "Animation.h"
#include "Mesh.h"
#include "Model.h"

Node::Node()
{
}

Node::~Node()
{
}


void Node::Update()
{
	Matrix matParent = Matrix::Identity;

	// 부모 노드가 있다면 부모 노드의 WorldMatrix를 곱해서 자신의 WorldMatrix를 만든다.
	if (m_parent != nullptr)
	{
		matParent = m_parent->m_matWorld;
	}
	else
	{
		matParent = m_owner->GetTransform();
	}

	m_matWorld = m_matLocal * matParent;

	for (auto& child : m_children)
		child->Update();
}

void Node::Render(ComPtr<ID3D11DeviceContext> deviceContext)
{


	for (auto& child : m_children)
		child->Render(deviceContext);
}
