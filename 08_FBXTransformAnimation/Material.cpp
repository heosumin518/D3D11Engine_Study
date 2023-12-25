#include "pch.h"
#include "Struct.h"
#include "Material.h"

Material::Material()
{
	
}

Material::~Material()
{
	
}

void Material::Init(CB_UseTextureMap& cbUseTextureMap, ComPtr<ID3D11Buffer> matBuffer, ComPtr<ID3D11BlendState> blendState)
{
	m_cbUseTextureMap = cbUseTextureMap;
	m_pUseTextureMapBuffer = matBuffer;
	m_alphaBlendState = blendState;
}

void Material::Render(ComPtr<ID3D11DeviceContext> deviceContext)
{
	for (int i = 0; i < TextureType::End; i++)
		deviceContext->PSSetShaderResources(i, 1, &m_textures[i]);

	m_cbUseTextureMap.isUseDiffuseMap = m_textures[Diffuse] != nullptr ? true : false;
	m_cbUseTextureMap.isUseNormalMap = m_textures[Normal] != nullptr ? true : false;
	m_cbUseTextureMap.isUseSpecularMap = m_textures[Specular] != nullptr ? true : false;
	m_cbUseTextureMap.isUseEmissiveMap = m_textures[Emissive] != nullptr ? true : false;
	m_cbUseTextureMap.isUseOpacityMap = m_textures[Opacity] != nullptr ? true : false;

	if (true == m_cbUseTextureMap.isUseOpacityMap)
		deviceContext->OMSetBlendState(m_alphaBlendState.Get(), nullptr, 0xffffffff);
	else
		deviceContext->OMSetBlendState(nullptr, nullptr, 0xffffffff);

	deviceContext->UpdateSubresource(m_pUseTextureMapBuffer.Get(), 0, nullptr, &m_cbUseTextureMap, 0, 0);

	deviceContext->PSSetConstantBuffers(3, 1, m_pUseTextureMapBuffer.GetAddressOf());
	deviceContext->VSSetConstantBuffers(3, 1, m_pUseTextureMapBuffer.GetAddressOf());
}

/*
m_CBMaterial.useDiffuseMap = model->GetMaterials()[mi]->GetDiffuseRV() != nullptr ? true : false;
m_CBMaterial.useNormalMap = model->GetMaterials()[mi]->GetNormalRV() != nullptr ? true : false;
m_CBMaterial.useSpecularMap = model->GetMaterials()[mi]->GetSpecularRV() != nullptr ? true : false;
m_CBMaterial.useEmissiveMap = model->GetMaterials()[mi]->GetEmissiveRV() != nullptr ? true : false;
m_CBMaterial.useOpacityMap = model->GetMaterials()[mi]->GetOpacityRV() != nullptr ? true : false;


*/