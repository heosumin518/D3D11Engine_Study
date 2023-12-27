#pragma once


struct CB_ConstantBuffer
{
	float specularPower;
	float ambient;
	float pad0[2];
};

static_assert((sizeof(CB_ConstantBuffer) % 16) == 0,
	"Constant Buffer size must be 16-byte aligned");

struct CB_BoolBuffer
{
	int isUseNormalMap;
	int isUseSpecularMap;
	int isUseGammaCorrection;
	int isUseDiffuseMap;
	int isUseEmissiveMap;
	int isUseOpacityMap;
	int pad0[2];
};

static_assert((sizeof(CB_BoolBuffer) % 16) == 0,
	"Constant Buffer size must be 16-byte aligned");

struct CB_TransformBuffer
{
	Matrix world = Matrix::Identity;
	Matrix view = Matrix::Identity;
	Matrix projection = Matrix::Identity;
};

static_assert((sizeof(CB_TransformBuffer) % 16) == 0,
	"Constant Buffer size must be 16-byte aligned");

struct CB_LightDirBuffer
{
	Vector4 lightDir;
	Vector4 lightColor;
	Vector4 worldCameraPos;
	Vector4 pad0[1];
};

static_assert((sizeof(CB_LightDirBuffer) % 16) == 0,
	"Constant Buffer size must be 16-byte aligned");