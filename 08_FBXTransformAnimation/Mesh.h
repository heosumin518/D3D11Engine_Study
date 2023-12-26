#pragma once



struct Vertex
{
	Vector3 position;
	Vector2 uv;
	Vector3 normal;
	Vector3 tangent;
};

class ModelLoader;
class Node;

class Mesh
{
public:
	Mesh();
	~Mesh();

	//void Create(ComPtr<ID3D11Device> device, aiMesh* srcMesh);

	void CreateVertexBuffer(ComPtr<ID3D11Device> device, vector<Vertex>& vertices);
	void CreateIndexBuffer(ComPtr<ID3D11Device> device, vector<WORD>& indices);

public:
	friend Node;
	friend ModelLoader;

private:
	string m_name;

	ComPtr<ID3D11Buffer> m_vertexBuffer;
	ComPtr<ID3D11Buffer> m_indexBuffer;

	Matrix* m_nodeWorld;

	UINT m_vertexCount = 0;
	UINT m_vertexBufferStride = 0;		// 버텍스 하나의 크기
	UINT m_vertexBufferOffset = 0;		// 버텍스 버퍼의 오프셋
	UINT m_indexCount = 0;
	UINT m_materialIndex = 0;
};

