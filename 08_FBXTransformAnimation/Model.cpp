#include "pch.h"
#include "Model.h"

Model::Model()
{
	
}

Model::~Model()
{
	
}

void Model::Init(ComPtr<ID3D11Device> device, ComPtr<ID3D11Buffer> modelData)
{
	m_CBNodeData = modelData;
	m_rootNode->Init(device, m_CBNodeData);
}

void Model::Update(ComPtr<ID3D11DeviceContext> deviceContext, float deltaTime)
{
	m_rootNode->Update(deviceContext, deltaTime);
}

//void Model::Update(float deltaTime)
//{
//
//
//	// 애니메이션 업데이트
//	for (const auto& anim : m_animations)
//		anim->Update(deltaTime);
//
//	// 노드 업데이트
//	for (const auto& node : m_nodes)
//		node->Update();
//
//}

//void Model::Render(ComPtr<ID3D11DeviceContext> deviceContext)
//{
//	for (size_t i = 0; i < m_meshes.size(); i++)
//	{
//		size_t mi = m_meshes[i]->m_materialIndex;
//
//		deviceContext->PSSetShaderResources(0, 1, m_materials[mi]->m_diffuseRV.GetAddressOf());
//		deviceContext->PSSetShaderResources(1, 1, m_materials[mi]->m_normalRV.GetAddressOf());
//		deviceContext->PSSetShaderResources(2, 1, m_materials[mi]->m_specularRV.GetAddressOf());
//		deviceContext->PSSetShaderResources(3, 1, m_materials[mi]->m_emissiveRV.GetAddressOf());
//		deviceContext->PSSetShaderResources(4, 1, m_materials[mi]->m_opacityRV.GetAddressOf());
//
//
//	}
//}

