#include "pch.h"
#include "Mesh.h"
#include "Bone.h"

Mesh::Mesh()
{
}

Mesh::~Mesh()
{
}

void Mesh::CreateVertexBuffer(ComPtr<ID3D11Device> device, vector<BoneWeightVertex>& vertices)
{
	m_vertexBufferStride = sizeof(BoneWeightVertex);
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

void Mesh::CreateIndexBuffer(ComPtr<ID3D11Device> device, vector<UINT>& indices)
{
	m_indexCount = indices.size();

	D3D11_BUFFER_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	desc.ByteWidth = sizeof(UINT) * m_indexCount;
	desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA data;
	ZeroMemory(&data, sizeof(data));
	data.pSysMem = indices.data();
	HR_T(device->CreateBuffer(&desc, &data, m_indexBuffer.GetAddressOf()));
}

void Mesh::UpdateMatrixPalette(vector<shared_ptr<Bone>> bones, Matrix* pMatrixPalette)
{
	assert(bones.size() < 128);

	for (UINT i = 0; i < bones.size(); i++)
	{
		Matrix boneNodeWorldMatrix = bones[i]->owner->GetNodeWorldTransform();
		boneNodeWorldMatrix = (bones[i]->offsetMatrix * boneNodeWorldMatrix).Transpose();

		pMatrixPalette[i] = boneNodeWorldMatrix;
	}
}
