#pragma once
#include "ModelLoader.h"
#include "Node.h"
#include "Struct.h"



class Mesh
{
public:
	Mesh();
	~Mesh();

	void CreateVertexBuffer(ComPtr<ID3D11Device> device, vector<BoneWeightVertex>& vertices);
	void CreateIndexBuffer(ComPtr<ID3D11Device> device, vector<WORD>& indices);

public:
	UINT GetIndexCount() const { return m_indexCount; }
	UINT GetMaterialIndex() const { return m_materialIndex; }
	const UINT* GetVertexBufferStride() { return &m_vertexBufferStride; }
	const UINT* GetVertexBufferOffset() { return &m_vertexBufferOffset; }

	ComPtr<ID3D11Buffer> GetIndexBuffer() { return m_indexBuffer; }
	ComPtr<ID3D11Buffer> GetVertexBuffer() { return m_vertexBuffer; }

	Matrix GetNodeTransform() { return m_connectedNode->m_world; }

private:
	friend ModelLoader;
	friend Model;

	ComPtr<ID3D11Buffer> m_vertexBuffer;
	ComPtr<ID3D11Buffer> m_indexBuffer;

	string m_name;

	UINT m_vertexCount = 0;
	UINT m_vertexBufferStride = 0;	// 버텍스 하나의 크기
	UINT m_vertexBufferOffset = 0;	// 버텍스 버퍼의 오프셋
	UINT m_indexCount = 0;			// 인덱스 개수
	UINT m_materialIndex = 0;		// 머터리얼 인덱스

	// ..
	shared_ptr<Node> m_connectedNode;
};

