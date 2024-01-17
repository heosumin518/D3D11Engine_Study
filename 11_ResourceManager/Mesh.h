#pragma once
#include "ModelLoader.h"
#include "Node.h"
#include "Struct.h"


// TODO: Mesh 는 이제 추상 클래스로 만들자. Static 과 Skeletal 로 분리해서 관리.
class Mesh
{
public:
	Mesh();
	~Mesh();

	template <typename T>
	void CreateVertexBuffer(ComPtr<ID3D11Device> device, vector<T>& vertices);

	void CreateIndexBuffer(ComPtr<ID3D11Device> device, vector<UINT>& indices);

public:
	UINT GetIndexCount() const { return m_indexCount; }
	UINT GetMaterialIndex() const { return m_materialIndex; }
	const UINT* GetVertexBufferStride() { return &m_vertexBufferStride; }
	const UINT* GetVertexBufferOffset() { return &m_vertexBufferOffset; }

	ComPtr<ID3D11Buffer> GetIndexBuffer() { return m_indexBuffer; }
	ComPtr<ID3D11Buffer> GetVertexBuffer() { return m_vertexBuffer; }

	void UpdateMatrixPalette(vector<shared_ptr<Bone>> bones, Matrix* pMatrixPalette);

	Matrix GetNodeTransform() { return m_connectedNode->m_world; }

private:
	friend ModelLoader;
	friend Model;

	vector<BoneWeightVertex> m_boneWeightVertices;	// TODO : static 과 skeletal 별로 다르게 가지고 있도록 하기.

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

template <typename T>
void Mesh::CreateVertexBuffer(ComPtr<ID3D11Device> device, vector<T>& vertices)
{
	m_vertexBufferStride = sizeof(T);
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
