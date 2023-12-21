#pragma once
#include "pch.h"

struct CB_ModelTransform
{
	Matrix World = Matrix::Identity;
};

static_assert((sizeof(CB_ModelTransform) % 16) == 0,
	"Constant Buffer size must be 16-byte aligned");

struct CB_UseTextureMap
{
	bool UseDiffuseMap = false;
	bool pad0[3];
	bool UseNormalMap = false;
	bool pad1[3];
	bool UseSpecularMap = false;
	bool pad2[3];
	bool UseEmissiveMap = false;
	bool pad3[3];
	bool UseOpacityMap = false;
	bool pad4[3];

	Vector3 pad5;
};

static_assert((sizeof(CB_UseTextureMap) % 16) == 0,
	"Constant Buffer size must be 16-byte aligned");

struct CB_Camera
{
	Vector4 Position;
};

static_assert((sizeof(CB_Camera) % 16) == 0,
	"Constant Buffer size must be 16-byte aligned");

struct CB_Coordinate
{
	Matrix World = Matrix::Identity;
	Matrix View = Matrix::Identity;
	Matrix Projection = Matrix::Identity;
};

static_assert((sizeof(CB_Coordinate) % 16) == 0,
	"Constant Buffer size must be 16-byte aligned");

struct CB_Light
{
	Vector4 Direction;
	Vector4 LightColor;
	float SpecularPower;
	Vector3 AmbientColor;
};

static_assert((sizeof(CB_Light) % 16) == 0,
	"Constant Buffer size must be 16-byte aligned");