#pragma once
#include "ModelLoader.h"

class Material
{
public:
	Material();
	virtual ~Material();

public:
	ComPtr<ID3D11ShaderResourceView> GetDiffuseRV() { return m_diffuseRV; }
	ComPtr<ID3D11ShaderResourceView> GetNormalRV() { return m_normalRV; }
	ComPtr<ID3D11ShaderResourceView> GetSpecularRV() { return m_specularRV; }
	ComPtr<ID3D11ShaderResourceView> GetEmissiveRV() { return m_emissiveRV; }
	ComPtr<ID3D11ShaderResourceView> GetOpacityRV() { return m_opacityRV; }

private:
	friend ModelLoader;
	friend Model;

	string m_name;

	// Shader Resource View
	ComPtr<ID3D11ShaderResourceView> m_diffuseRV = nullptr;		// diffuse ∏Æº“Ω∫ ∫‰
	ComPtr<ID3D11ShaderResourceView> m_normalRV = nullptr;		// normal ∏Æº“Ω∫ ∫‰
	ComPtr<ID3D11ShaderResourceView> m_specularRV = nullptr;	// specular ∏Æº“Ω∫ ∫‰
	ComPtr<ID3D11ShaderResourceView> m_emissiveRV = nullptr;	// ¿ÃπÃΩ√∫Í∏  ∏Æº“Ω∫ ∫‰.	
	ComPtr<ID3D11ShaderResourceView> m_opacityRV = nullptr;		// ≈ı∏Ì∏  ∏Æº“Ω∫ ∫‰.	
};

