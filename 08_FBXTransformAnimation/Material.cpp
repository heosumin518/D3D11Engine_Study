#include "pch.h"
#include "Material.h"

void Material::Create(ComPtr<ID3D11Device> device, aiMaterial* material)
{
	HRESULT hr = CoInitializeEx(nullptr, COINITBASE_MULTITHREADED);
	if (FAILED(hr))
	{
	}// error

	// Diffuse
	aiString texturePath;
	wstring basePath = L"../Resources/";
	filesystem::path path;
	wstring finalPath;
	string name = material->GetName().C_Str();

	aiColor3D color(1.f, 1.f, 1.f);
	if (AI_SUCCESS == material->Get(AI_MATKEY_COLOR_DIFFUSE, color))
		m_color = Color{ color.r, color.g, color.b, 1 };

	if (AI_SUCCESS == material->GetTexture(aiTextureType_DIFFUSE, 0, &texturePath))
	{
		path = ToWString(string(texturePath.C_Str()));
		finalPath = basePath + path.filename().wstring();
		HR_T(CreateTextureFromFile(device.Get(), finalPath.c_str(), m_diffuseSRV.GetAddressOf()));
	}

	if (AI_SUCCESS == material->GetTexture(aiTextureType_NORMALS, 0, &texturePath))
	{
		path = ToWString(string(texturePath.C_Str()));
		finalPath = basePath + path.filename().wstring();
		HR_T(CreateTextureFromFile(device.Get(), finalPath.c_str(), m_normalSRV.GetAddressOf()));
	}

	if (AI_SUCCESS == material->GetTexture(aiTextureType_SPECULAR, 0, &texturePath))
	{
		path = ToWString(string(texturePath.C_Str()));
		finalPath = basePath + path.filename().wstring();
		HR_T(CreateTextureFromFile(device.Get(), finalPath.c_str(), m_specularSRV.GetAddressOf()));
	}

	if (AI_SUCCESS == material->GetTexture(aiTextureType_EMISSIVE, 0, &texturePath))
	{
		path = ToWString(string(texturePath.C_Str()));
		finalPath = basePath + path.filename().wstring();
		HR_T(CreateTextureFromFile(device.Get(), finalPath.c_str(), m_emissiveSRV.GetAddressOf()));
	}

	if (AI_SUCCESS == material->GetTexture(aiTextureType_OPACITY, 0, &texturePath))
	{
		path = ToWString(string(texturePath.C_Str()));
		finalPath = basePath + path.filename().wstring();
		HR_T(CreateTextureFromFile(device.Get(), finalPath.c_str(), m_opacitySRV.GetAddressOf()));
	}
}