#pragma once
#include "ModelLoader.h"

// TODO: ÅØ½ºÃ³ Å¬·¡½º ¸¸µé¾î¼­ ¸ñ·Ï °ü¸®.
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
	ComPtr<ID3D11ShaderResourceView> GetMetalnessRV() { return m_metalnessRV; }
	ComPtr<ID3D11ShaderResourceView> GetRoughnessRV() { return m_roughnessRV; }

private:
	friend ModelLoader;
	friend Model;

	string m_name;

	// Shader Resource View
	ComPtr<ID3D11ShaderResourceView> m_diffuseRV = nullptr;		// diffuse ¸®¼Ò½º ºä
	ComPtr<ID3D11ShaderResourceView> m_normalRV = nullptr;		// normal ¸®¼Ò½º ºä
	ComPtr<ID3D11ShaderResourceView> m_specularRV = nullptr;	// specular ¸®¼Ò½º ºä
	ComPtr<ID3D11ShaderResourceView> m_emissiveRV = nullptr;	// ÀÌ¹Ì½Ãºê¸Ê ¸®¼Ò½º ºä.	
	ComPtr<ID3D11ShaderResourceView> m_opacityRV = nullptr;		// Åõ¸í¸Ê ¸®¼Ò½º ºä.	
	ComPtr<ID3D11ShaderResourceView> m_metalnessRV = nullptr;	// ¸ÞÅ»¸Ê ¸®¼Ò½º ºä.	
	ComPtr<ID3D11ShaderResourceView> m_roughnessRV = nullptr;	// ·¯ÇÁ´Ï½º¸Ê ¸®¼Ò½º ºä.	
};

