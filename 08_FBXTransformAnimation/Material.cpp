#include "pch.h"
#include "Struct.h"
#include "Material.h"

Material::Material()
{
	
}

Material::~Material()
{
	
}

void Material::Render(ComPtr<ID3D11DeviceContext> deviceContext)
{
	//deviceContext->PSSetShaderResources(0, 1, m_diffuseRV.GetAddressOf());
	//deviceContext->PSSetShaderResources(1, 1, m_normalRV.GetAddressOf());
	//deviceContext->PSSetShaderResources(2, 1, m_specularRV.GetAddressOf());
	//deviceContext->PSSetShaderResources(3, 1, m_emissiveRV.GetAddressOf());
	//deviceContext->PSSetShaderResources(4, 1, m_opacityRV.GetAddressOf());

	//m_CBUseTextureMap.UseDiffuseMap = m_diffuseRV != nullptr ? true : false;
	//m_CBUseTextureMap.UseNormalMap = m_normalRV != nullptr ? true : false;
	//m_CBUseTextureMap.UseSpecularMap = m_specularRV != nullptr ? true : false;
	//m_CBUseTextureMap.UseEmissiveMap = m_emissiveRV != nullptr ? true : false;
	//m_CBUseTextureMap.UseOpacityMap = m_opacityRV != nullptr ? true : false;

	//if (m_CBUseTextureMap.UseOpacityMap && m_alphaBlendState != nullptr)
	//	deviceContext->OMSetBlendState(m_alphaBlendState.Get(), nullptr, 0xffffffff); // 알파블렌드 상태설정 , 다른옵션은 기본값
	//else
	//	deviceContext->OMSetBlendState(nullptr, nullptr, 0xffffffff);	// 설정해제 , 다른옵션은 기본값

	//deviceContext->UpdateSubresource(m_CBUseTextureMapBuffer.Get(), 0, nullptr, &m_CBUseTextureMap, 0, 0);

	//deviceContext->PSSetConstantBuffers(3, 1, m_CBUseTextureMapBuffer.GetAddressOf());
	//deviceContext->VSSetConstantBuffers(3, 1, m_CBUseTextureMapBuffer.GetAddressOf());
}
