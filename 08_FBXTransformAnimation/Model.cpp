#include "pch.h"
#include "Model.h"
#include "Node.h"
#include "Mesh.h"
#include "Material.h"
#include "Animation.h"
#include "ModelLoader.h"

Model::Model()
{
	m_animation = make_shared<Animation>();
	m_rootNode = make_shared<Node>();
}

Model::~Model()
{
	
}

//void Model::ReadFile(ComPtr<ID3D11Device> device, const char* filePath)
//{
//	Assimp::Importer importer;
//
//	m_scene = importer.ReadFile(filePath,
//		aiProcess_ConvertToLeftHanded |	// DX용 왼손좌표계 변환
//		aiProcess_Triangulate | // vertex 삼각형 으로 출력
//		aiProcess_GenUVCoords |	// 텍스처 좌표 생성
//		aiProcess_GenNormals |	// Normal 정보 생성
//		aiProcess_CalcTangentSpace // 탄젠트 벡터 생성
//	);
//	assert(m_scene != nullptr);
//
//	m_meshes.resize(m_scene->mNumMeshes);
//	m_materials.resize(m_scene->mNumMaterials);
//
//	// fbx 모델의 텍스쳐맵 정보 가져오기
//	for (UINT i = 0; i < m_scene->mNumMaterials; i++)
//	{
//		m_materials[i] = make_shared<Material>();
//		m_materials[i]->Create(device, m_scene->mMaterials[i]);
//	}
//
//	// 각 애니메이션 벡터에 애니메이션 클립 생성하고 그 안에서 정보들 바인딩
//	for (UINT i = 0; i < m_scene->mNumAnimations; i++)
//	{
//		if (m_scene->mAnimations[i] != nullptr)
//			m_animation->Create(m_scene->mAnimations[i]);
//	}
//
//	// 노드에 애니메이션 바인딩
//	m_rootNode->Create(shared_from_this(), m_scene->mRootNode, m_animation);
//
//	// vertex, index 정보 바인딩
//	for (UINT i = 0; i < m_scene->mNumMeshes; i++)
//	{
//		m_meshes[i] = make_shared<Mesh>();
//		m_meshes[i]->Create(device, m_scene->mMeshes[i]);
//	}
//}
