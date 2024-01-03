#pragma once

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