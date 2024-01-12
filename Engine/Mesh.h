#pragma once
#include "ModelLoader.h"
#include "Node.h"

// static mesh
struct Vertex
{
	Vector3 position;
	Vector2 uv;
	Vector3 normal;
	Vector3 tangent;
};

// skeletal mesh
struct BoneWeightVertex
{
	Vector3 position;
	Vector2 uv;
	Vector3 normal;
	Vector3 tangent;

	// 참조하는 본의 인덱스의 범위는 최대 128개로 일단 처리.
	int blendIndices[4] = {};	// 영향받는 본의 수는 최대 4개로 제한.

	// 버텍스들이 참조하는 본위 weight 값.
	// 최대 4개를 참조하니 weight 값도 4개까지 있게 된다.
	float blendWeights[4] = {}; // 가중치의 총합은 1이 되어야 한다.

	void AddBoneData(int boneIndex, float weight)
	{
		// 적어도 하나는 데이터가 비어있어야 한다.
		assert(blendWeights[0] == 0.f || blendWeights[1] == 0.f ||
			blendWeights[2] == 0.f || blendWeights[3] == 0.f);

		for (UINT i = 0; i < 4; i++)
		{
			if (blendWeights[i] == 0.f)
			{
				blendIndices[i] = boneIndex;
				blendWeights[i] = weight;
				return;
			}
		}
	}
};

namespace Engine
{
	class Mesh
	{
	public:
		Mesh();
		~Mesh();

		void CreateVertexBuffer(ComPtr<ID3D11Device> device, vector<BoneWeightVertex>& vertices);
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

		vector<BoneWeightVertex> m_boneWeightVertices;

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
}


