#include "pch.h"
#include "Node.h"

#include "Animation.h"
#include "Mesh.h"

Node::Node()
{
}

Node::~Node()
{
}

void Node::Init(ComPtr<ID3D11Buffer> modelBuffer)
{
	m_nodeBuffer = modelBuffer;

	for (auto& child : m_children)
	{
		if (child != nullptr)
			child->Init(modelBuffer);
	}
}

void Node::Update(float progressTime)
{
	// 노드의 애니메이션이 있다면 애니메이션을 업데이트한다.
	if (m_nodeAnimation != nullptr)
	{
		Vector3 position, scale;
		Quaternion rotation;

		m_nodeAnimation->Evaluate(progressTime, position, rotation, scale);

		m_matLocal = Matrix::CreateScale(scale) *
			Matrix::CreateFromQuaternion(rotation) *
			Matrix::CreateTranslation(position);
	}

	// 부모 노드가 있다면 부모 노드의 WorldMatrix를 곱해서 자신의 WorldMatrix를 만든다.
	if (m_parent != nullptr)
	{
		m_matWorld = m_matLocal * m_parent->m_matWorld;
		m_CBNodeTransform.World = m_matWorld;
	}
	else
	{
		m_matWorld = m_matLocal;
		m_CBNodeTransform.World = m_matWorld;
	}

	for (auto& child : m_children)
		child->Update(progressTime);
}

void Node::Render(ComPtr<ID3D11DeviceContext> deviceContext)
{
	assert(m_mesh != nullptr);

	auto matrix = m_CBNodeTransform.World.Transpose();
	deviceContext->UpdateSubresource(m_nodeBuffer.Get(), 0, nullptr, &matrix, 0, 0);

	deviceContext->VSSetConstantBuffers(4, 1, m_nodeBuffer.GetAddressOf());
	deviceContext->PSSetConstantBuffers(4, 1, m_nodeBuffer.GetAddressOf());

	m_mesh->Render(deviceContext);

	for (auto& child : m_children)
		child->Render(deviceContext);
}
