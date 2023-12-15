#include "pch.h"
#include "Mesh.h"

void Mesh::Create(ComPtr<ID3D11Device> device, aiMesh* mesh)
{
	m_materialIndex = mesh->mMaterialIndex;
	m_name = mesh->mName.C_Str();



}
