#include "pch.h"
#include "Model.h"

bool Model::ReadFile(ComPtr<ID3D11Device> device, const char* filePath)
{
	Assimp::Importer importer;

	importer.SetPropertyBool(AI_CONFIG_IMPORT_FBX_PRESERVE_PIVOTS, 0);	// $assimp_fbx$ 노드 생성안함

	const aiScene* scene = importer.ReadFile(
		filePath,
		aiProcess_ConvertToLeftHanded |	// DX용 왼손좌표계 변환
		aiProcess_Triangulate | // vertex 삼각형 으로 출력
		aiProcess_GenUVCoords |	// 텍스처 좌표 생성
		aiProcess_GenNormals |	// Normal 정보 생성
		aiProcess_CalcTangentSpace // 탄젠트 벡터 생성
	);
	assert(scene != nullptr);

	m_materials.resize(scene->mNumMaterials);
	for (UINT i = 0; i < scene->mNumMaterials; i++)
		m_materials[i].Create(device, scene->mMaterials[i]);

	m_meshes.resize(scene->mNumMeshes);
	for (UINT i = 0; i < scene->mNumMeshes; i++)
		m_meshes[i].Create(device, scene->mMeshes[i], )

}
