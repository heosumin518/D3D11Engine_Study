#pragma once
#include "ModelLoader.h"
#include "Struct.h"

enum TextureType
{
	Diffuse, Normal, Specular, Emissive, Opacity, End,
};

class Material
{
public:
	Material();
	virtual ~Material();

	void Create(ComPtr<ID3D11Device> device, aiMaterial* srcMaterial);

public:
	ComPtr<ID3D11ShaderResourceView> GetDiffuseRV() { return m_diffuseRV; }
	ComPtr<ID3D11ShaderResourceView> GetNormalRV() { return m_normalRV; }
	ComPtr<ID3D11ShaderResourceView> GetSpecularRV() { return m_specularRV; }
	ComPtr<ID3D11ShaderResourceView> GetEmissiveRV() { return m_emissiveRV; }
	ComPtr<ID3D11ShaderResourceView> GetOpacityRV() { return m_opacityRV; }

private:
	string m_name;

	// Shader Resource View
	ComPtr<ID3D11ShaderResourceView> m_diffuseRV = nullptr;		// diffuse ∏Æº“Ω∫ ∫‰
	ComPtr<ID3D11ShaderResourceView> m_normalRV = nullptr;		// normal ∏Æº“Ω∫ ∫‰
	ComPtr<ID3D11ShaderResourceView> m_specularRV = nullptr;	// specular ∏Æº“Ω∫ ∫‰
	ComPtr<ID3D11ShaderResourceView> m_emissiveRV = nullptr;	// ¿ÃπÃΩ√∫Í∏  ∏Æº“Ω∫ ∫‰.	
	ComPtr<ID3D11ShaderResourceView> m_opacityRV = nullptr;		// ≈ı∏Ì∏  ∏Æº“Ω∫ ∫‰.

};

