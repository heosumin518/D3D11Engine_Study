#include "pch.h"
#include "Mesh.h"

Mesh::Mesh(ComPtr<ID3D11Device> device)
	: m_device(device)
{
}

Mesh::~Mesh()
{
}

void Mesh::CreateVertexBuffer(Vertex* vertices, UINT vertexCount)
{
	m_vertexBufferStride = sizeof(Vertex);
	m_vertexCount = vertexCount;

	D3D11_BUFFER_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	desc.ByteWidth = m_vertexBufferStride * m_vertexCount;
	desc.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA data;
	ZeroMemory(&data, sizeof(data));
	data.pSysMem = vertices;
	HR_T(m_device->CreateBuffer(&desc, &data, m_vertexBuffer.GetAddressOf()));
}

void Mesh::CreateIndexBuffer(WORD* indices, UINT indexCount)
{

}

void Mesh::Create(aiMesh* mesh)
{
}
