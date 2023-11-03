#pragma once

struct Vertex
{
	Vector3 position;
	Vector2 uv;
	Vector3 normal;
	Vector3 tangent;
};

static aiMesh;
class Mesh
{
public:
	Mesh(ComPtr<ID3D11Device> device);
	~Mesh();

	void CreateVertexBuffer(Vertex* vertices, UINT vertexCount);
	void CreateIndexBuffer(WORD* indices, UINT indexCount);
	void Create(aiMesh* mesh);

private:
	ComPtr<ID3D11Device> m_device;
	ComPtr<ID3D11Buffer> m_vertexBuffer;
	ComPtr<ID3D11Buffer> m_indexBuffer;

	UINT m_vertexCount = 0;
	UINT m_vertexBufferStride = 0;	// 버텍스 하나의 크기
	UINT m_vertexBufferOffset = 0;	// 버텍스 버퍼의 오프셋
	UINT m_indexCount = 0;	// 인덱스 개수
	UINT m_indexBufferStride = 0;
	UINT m_indexBufferOffset = 0;
	UINT m_materialIndex = 0;	// 머터리얼 인덱스

};

