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

	void Init(CB_UseTextureMap& cbUseTextureMap, ComPtr<ID3D11Buffer> matBuffer, ComPtr<ID3D11BlendState> blendState);
	void Render(ComPtr<ID3D11DeviceContext> deviceContext);

public:
	ComPtr<ID3D11ShaderResourceView> GetDiffuseRV() { return m_diffuseRV; }
	ComPtr<ID3D11ShaderResourceView> GetNormalRV() { return m_normalRV; }
	ComPtr<ID3D11ShaderResourceView> GetSpecularRV() { return m_specularRV; }
	ComPtr<ID3D11ShaderResourceView> GetEmissiveRV() { return m_emissiveRV; }
	ComPtr<ID3D11ShaderResourceView> GetOpacityRV() { return m_opacityRV; }

private:
	friend ModelLoader;
	friend Model;


	// Shader Resource View
	ComPtr<ID3D11ShaderResourceView> m_diffuseRV = nullptr;		// diffuse ∏Æº“Ω∫ ∫‰
	ComPtr<ID3D11ShaderResourceView> m_normalRV = nullptr;		// normal ∏Æº“Ω∫ ∫‰
	ComPtr<ID3D11ShaderResourceView> m_specularRV = nullptr;	// specular ∏Æº“Ω∫ ∫‰
	ComPtr<ID3D11ShaderResourceView> m_emissiveRV = nullptr;	// ¿ÃπÃΩ√∫Í∏  ∏Æº“Ω∫ ∫‰.	
	ComPtr<ID3D11ShaderResourceView> m_opacityRV = nullptr;		// ≈ı∏Ì∏  ∏Æº“Ω∫ ∫‰.

	// now use 23/12/22
	string name;
	ComPtr<ID3D11ShaderResourceView> m_textures[TextureType::End];

	CB_UseTextureMap m_cbUseTextureMap;
	ComPtr<ID3D11Buffer> m_pUseTextureMapBuffer;

	ComPtr<ID3D11BlendState> m_alphaBlendState;
};

