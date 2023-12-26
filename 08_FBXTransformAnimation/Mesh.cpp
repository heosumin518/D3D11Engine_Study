#include "pch.h"
#include "Mesh.h"
#include "Model.h"
#include "Node.h"
#include "ModelLoader.h"

Mesh::Mesh()
{
}

Mesh::~Mesh()
{
}
//
//void Mesh::Create(ComPtr<ID3D11Device> device, aiMesh* srcMesh)
//{
//	shared_ptr<Mesh> mesh = make_shared<Mesh>();
//
//	m_materialIndex = srcMesh->mMaterialIndex;
//
//	// 버텍스 정보 생성
//	vector<Vertex> vertices;
//	for (UINT v = 0; v < srcMesh->mNumVertices; v++)
//	{
//		// Vertex
//		Vertex vertex;
//		::memcpy(&vertex.position, &srcMesh->mVertices[v], sizeof(srcMesh->mVertices[v]));
//
//		// UV
//		if (srcMesh->HasTextureCoords(0))
//			::memcpy(&vertex.uv, &srcMesh->mTextureCoords[0][v], sizeof(Vector2));
//
//		// Normal
//		if (srcMesh->HasNormals())
//			::memcpy(&vertex.normal, &srcMesh->mNormals[v], sizeof(srcMesh->mNormals[v]));
//
//		// Tangent
//		if (srcMesh->HasTangentsAndBitangents())
//			::memcpy(&vertex.tangent, &srcMesh->mTangents[v], sizeof(srcMesh->mTangents[v]));
//
//		vertices.push_back(vertex);
//	}
//	mesh->CreateVertexBuffer(device, vertices);
//
//	// 인덱스 정보 생성
//	vector<WORD> indices;
//	for (UINT f = 0; f < srcMesh->mNumFaces; f++)
//	{
//		indices.push_back(srcMesh->mFaces[f].mIndices[0]);
//		indices.push_back(srcMesh->mFaces[f].mIndices[1]);
//		indices.push_back(srcMesh->mFaces[f].mIndices[2]);
//	}
//	mesh->CreateIndexBuffer(device, indices);
//}

void Mesh::CreateVertexBuffer(ComPtr<ID3D11Device> device, vector<Vertex>& vertices)
{
	m_vertexBufferStride = sizeof(Vertex);
	m_vertexBufferOffset = 0;
	m_vertexCount = vertices.size();

	D3D11_BUFFER_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	desc.ByteWidth = m_vertexBufferStride * vertices.size();
	desc.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA data;
	ZeroMemory(&data, sizeof(data));
	data.pSysMem = vertices.data();
	HR_T(device->CreateBuffer(&desc, &data, m_vertexBuffer.GetAddressOf()));
}

void Mesh::CreateIndexBuffer(ComPtr<ID3D11Device> device, vector<WORD>& indices)
{
	m_indexCount = indices.size();

	D3D11_BUFFER_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	desc.ByteWidth = sizeof(WORD) * m_indexCount;
	desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA data;
	ZeroMemory(&data, sizeof(data));
	data.pSysMem = indices.data();
	HR_T(device->CreateBuffer(&desc, &data, m_indexBuffer.GetAddressOf()));
}
