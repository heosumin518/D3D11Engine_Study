#include "pch.h"
#include "Model.h"

Model::Model()
{
	
}

Model::~Model()
{
	
}

void Model::Init(CB_Transform& nodeTransform, ComPtr<ID3D11Buffer> nodeBuffer,
	CB_UseTextureMap& matTransform, ComPtr<ID3D11Buffer> matBuffer, ComPtr<ID3D11BlendState> blendState)
{
	m_root->Init(nodeTransform, nodeBuffer, matTransform, matBuffer, blendState);
}


void Model::Update(float deltaTime)
{
	Matrix scale = XMMatrixScaling(m_scale.x, m_scale.y, m_scale.z);
	Matrix rotation = XMMatrixRotationX(m_rotation.x) * XMMatrixRotationY(m_rotation.y) * XMMatrixRotationZ(m_rotation.z);
	Matrix translate = XMMatrixTranslation(m_position.x, m_position.y, m_position.z);
	m_transform = scale * rotation * translate;

	for (auto& anim : m_animations)
		anim->Update(deltaTime);

	m_root->Update();
}

void Model::Render(ComPtr<ID3D11DeviceContext> deviceContext, CB_Transform& nodeTransform)
{
	m_root->Render(deviceContext, nodeTransform);
}
