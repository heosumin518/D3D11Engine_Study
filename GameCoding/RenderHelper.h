#pragma once

// 쉐이더에 넘겨줄 데이터들
struct CameraData
{
	Matrix matView = Matrix::Identity;
	Matrix matProjection = Matrix::Identity;
};

struct TransformData
{
	Matrix matWorld = Matrix::Identity;
};

struct AnimationData
{
	Vec2 spriteOffset;
	Vec2 spriteSize;
	Vec2 textureSize;
	float useAnimation;		
	float padding;		// 16 byte 를 맞춰주기 위한 더미 데이터
};