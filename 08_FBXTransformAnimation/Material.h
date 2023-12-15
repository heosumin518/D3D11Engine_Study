#pragma once

struct CB_Material
{
	Vector4 ambient = { 1.0f,1.0f,1.0f,1.0f };
	Vector4 diffuse = { 1.0f,1.0f,1.0f,1.0f };
	Vector4 specular = { 1.0f,1.0f,1.0f,1.0f };
	Vector4 emissive = { 1.0f,1.0f,1.0f,1.0f };
	float specularPower = 80;
	int bUseDiffuseMap = true;
	int bUseNormalMap = true;
	int bUseSpecularMap = true;
	int bUseEmissiveMap = true;
	int bUseOpacityMap = true;
	Vector2 pad0;
};

static_assert((sizeof(CB_Material) % 16) == 0,
	"Constant Buffer size must be 16-byte aligned");

class Material
{
public:
	Material() {}
	~Material() {}

	void Create(ComPtr<ID3D11Device> device, aiMaterial* material);
	void ApplyDeviceContext(ComPtr<ID3D11DeviceContext> deviceContext, shared_ptr<CB_Material> cpuCBMaterial, ComPtr<ID3D11Buffer> gpuCBMaterial, ComPtr<ID3D11BlendState> alphaBlendState);

private:
	Color m_color;
	ComPtr<ID3D11ShaderResourceView> m_diffuseSRV;
	ComPtr<ID3D11ShaderResourceView> m_normalSRV;
	ComPtr<ID3D11ShaderResourceView> m_specularSRV;
	ComPtr<ID3D11ShaderResourceView> m_emissiveSRV;
	ComPtr<ID3D11ShaderResourceView> m_opacitySRV;
};

