#pragma once
#include "ModelLoader.h"

class Material
{
public:
	Material();
	virtual ~Material();

	void Render(ComPtr<ID3D11DeviceContext> deviceContext);

public:
	void SetConstantBufferData(CB_UseTextureMap data, ComPtr<ID3D11Buffer> buffer, ComPtr<ID3D11BlendState> alphaBlendState) { m_CBUseTextureMap = data; m_CBUseTextureMapBuffer = buffer; m_alphaBlendState = alphaBlendState; }

public:
	ComPtr<ID3D11ShaderResourceView> GetDiffuseRV() { return m_diffuseRV; }
	ComPtr<ID3D11ShaderResourceView> GetNormalRV() { return m_normalRV; }
	ComPtr<ID3D11ShaderResourceView> GetSpecularRV() { return m_specularRV; }
	ComPtr<ID3D11ShaderResourceView> GetEmissiveRV() { return m_emissiveRV; }
	ComPtr<ID3D11ShaderResourceView> GetOpacityRV() { return m_opacityRV; }

private:
	friend ModelLoader;
	friend Model;

	string name;

	CB_UseTextureMap m_CBUseTextureMap;
	ComPtr<ID3D11Buffer> m_CBUseTextureMapBuffer;

	ComPtr<ID3D11BlendState> m_alphaBlendState;

	// Shader Resource View
	ComPtr<ID3D11ShaderResourceView> m_diffuseRV = nullptr;		// diffuse ∏Æº“Ω∫ ∫‰
	ComPtr<ID3D11ShaderResourceView> m_normalRV = nullptr;		// normal ∏Æº“Ω∫ ∫‰
	ComPtr<ID3D11ShaderResourceView> m_specularRV = nullptr;	// specular ∏Æº“Ω∫ ∫‰
	ComPtr<ID3D11ShaderResourceView> m_emissiveRV = nullptr;	// ¿ÃπÃΩ√∫Í∏  ∏Æº“Ω∫ ∫‰.	
	ComPtr<ID3D11ShaderResourceView> m_opacityRV = nullptr;		// ≈ı∏Ì∏  ∏Æº“Ω∫ ∫‰.	
};

