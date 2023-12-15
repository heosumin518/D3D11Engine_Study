#pragma once

struct Vertex
{
	Vector3 position;
	Vector2 uv;
	Vector3 normal;
	Vector3 tangent;
};

class Mesh
{
public:
	Mesh() {}
	~Mesh() {}

public:
	void Create(ComPtr<ID3D11Device> device, aiMesh* mesh);
	void Render(ComPtr<ID3D11DeviceContext> deviceContext);

	void CreateVertexBuffer(ComPtr<ID3D11Device> device, shared_ptr<Vertex> vertices, UINT vertexCount);
	void CreateIndexBuffer(ComPtr<ID3D11Device> device, shared_ptr<WORD> indices, UINT indexCount);

	void SetNodeWorldPtr(shared_ptr<Matrix> world) { m_nodeWorldTransform = world; }

private:
	vector<Vertex> m_vertices;
	vector<WORD> m_indices;

	shared_ptr<Matrix> m_nodeWorldTransform;
	ComPtr<ID3D11Buffer> m_vertexBuffer;
	ComPtr<ID3D11Buffer> m_indexBuffer;

	UINT m_vertexCount = 0;
	UINT m_vertexBufferStride = 0;	// 버텍스 하나의 크기
	UINT m_vertexBufferOffset = 0;	// 버텍스 버퍼의 오프셋
	UINT m_indexCount = 0;		
	UINT m_materialIndex = 0;
	string m_name;
};

