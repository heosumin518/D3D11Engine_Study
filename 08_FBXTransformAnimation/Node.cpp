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

void Node::Init(CB_Transform& nodeTransform, ComPtr<ID3D11Buffer> nodeBuffer,
	CB_UseTextureMap& matTransform, ComPtr<ID3D11Buffer> matBuffer, ComPtr<ID3D11BlendState> blendState)
{
	m_transform = nodeTransform;
	m_buffer = nodeBuffer;

	for (auto& mesh : m_meshs)
		mesh->Init(matTransform, matBuffer, blendState);

	for (auto& child : m_children)
		child->Init(nodeTransform, nodeBuffer, matTransform, matBuffer, blendState);
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

void Node::Render(ComPtr<ID3D11DeviceContext> deviceContext, CB_Transform& nodeTransform)
{
	nodeTransform.world = XMMatrixTranspose(m_matWorld);
	deviceContext->VSSetConstantBuffers(0, 1, m_buffer.GetAddressOf());
	deviceContext->PSSetConstantBuffers(0, 1, m_buffer.GetAddressOf());
	deviceContext->UpdateSubresource(m_buffer.Get(), 0, nullptr, &nodeTransform, 0, 0);

	for (auto& mesh : m_meshs)
		mesh->Render(deviceContext);

	for (auto& child : m_children)
		child->Render(deviceContext, nodeTransform);
}
