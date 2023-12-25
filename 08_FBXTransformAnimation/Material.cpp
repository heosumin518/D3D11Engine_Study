#include "pch.h"
#include "Struct.h"
#include "Material.h"

Material::Material()
{
	
}

Material::~Material()
{
	
}

void Material::Create(ComPtr<ID3D11Device> device, aiMaterial* srcMaterial)
{
	m_name = srcMaterial->GetName().C_Str();

	wstring basePath = L"../Resources/";
	wstring finalPath;
	aiString texturePath;

	filesystem::path path;
	if (AI_SUCCESS == srcMaterial->GetTexture(aiTextureType_DIFFUSE, 0, &texturePath))
	{
		path = ToWString(string(texturePath.C_Str()));
		finalPath = basePath + path.filename().wstring();
		HR_T(CreateTextureFromFile(device.Get(), finalPath.c_str(), m_diffuseRV.GetAddressOf()));
	}

	if (AI_SUCCESS == srcMaterial->GetTexture(aiTextureType_NORMALS, 0, &texturePath))
	{
		path = ToWString(string(texturePath.C_Str()));
		finalPath = basePath + path.filename().wstring();
		HR_T(CreateTextureFromFile(device.Get(), finalPath.c_str(), m_normalRV.GetAddressOf()));
	}

	if (AI_SUCCESS == srcMaterial->GetTexture(aiTextureType_SPECULAR, 0, &texturePath))
	{
		path = ToWString(string(texturePath.C_Str()));
		finalPath = basePath + path.filename().wstring();
		HR_T(CreateTextureFromFile(device.Get(), finalPath.c_str(), m_specularRV.GetAddressOf()));
	}

	if (AI_SUCCESS == srcMaterial->GetTexture(aiTextureType_EMISSIVE, 0, &texturePath))
	{
		path = ToWString(string(texturePath.C_Str()));
		finalPath = basePath + path.filename().wstring();
		HR_T(CreateTextureFromFile(device.Get(), finalPath.c_str(), m_emissiveRV.GetAddressOf()));
	}

	if (AI_SUCCESS == srcMaterial->GetTexture(aiTextureType_OPACITY, 0, &texturePath))
	{
		path = ToWString(string(texturePath.C_Str()));
		finalPath = basePath + path.filename().wstring();
		HR_T(CreateTextureFromFile(device.Get(), finalPath.c_str(), m_opacityRV.GetAddressOf()));
	}
}
