#include "pch.h"
#include "Model.h"
#include "Node.h"
#include "Mesh.h"
#include "Material.h"
#include "Animation.h"

Model::Model()
{
	
}

Model::~Model()
{
	
}

bool Model::ReadFile(ComPtr<ID3D11Device> device, const char* filePath)
{
	Assimp::Importer importer;

	m_scene = importer.ReadFile(filePath,
		aiProcess_ConvertToLeftHanded |	// DX¿ë ¿Þ¼ÕÁÂÇ¥°è º¯È¯
		aiProcess_Triangulate | // vertex »ï°¢Çü À¸·Î Ãâ·Â
		aiProcess_GenUVCoords |	// ÅØ½ºÃ³ ÁÂÇ¥ »ý¼º
		aiProcess_GenNormals |	// Normal Á¤º¸ »ý¼º
		aiProcess_CalcTangentSpace // ÅºÁ¨Æ® º¤ÅÍ »ý¼º
	);
	assert(m_scene != nullptr);

	m_meshes.resize(m_scene->mNumMeshes);
	m_materials.resize(m_scene->mNumMaterials);

	for (UINT i = 0; i < m_scene->mNumMaterials; i++)
		m_materials[i]->Create(device, m_scene->mMaterials[i]);



}
