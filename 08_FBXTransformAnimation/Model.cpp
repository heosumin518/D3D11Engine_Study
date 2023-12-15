#include "pch.h"
#include "Model.h"

bool Model::ReadFile(ComPtr<ID3D11Device> device, const char* filePath)
{
	Assimp::Importer importer;

	importer.SetPropertyBool(AI_CONFIG_IMPORT_FBX_PRESERVE_PIVOTS, 0);	// $assimp_fbx$ ��� ��������

	const aiScene* scene = importer.ReadFile(
		filePath,
		aiProcess_ConvertToLeftHanded |	// DX�� �޼���ǥ�� ��ȯ
		aiProcess_Triangulate | // vertex �ﰢ�� ���� ���
		aiProcess_GenUVCoords |	// �ؽ�ó ��ǥ ����
		aiProcess_GenNormals |	// Normal ���� ����
		aiProcess_CalcTangentSpace // ź��Ʈ ���� ����
	);
	assert(scene != nullptr);

	m_materials.resize(scene->mNumMaterials);
	for (UINT i = 0; i < scene->mNumMaterials; i++)
		m_materials[i].Create(device, scene->mMaterials[i]);

	m_meshes.resize(scene->mNumMeshes);
	for (UINT i = 0; i < scene->mNumMeshes; i++)
		m_meshes[i].Create(device, scene->mMeshes[i], )

}
