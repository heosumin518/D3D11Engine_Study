#pragma once
#include "pch.h"

struct CB_Transform
{
	Matrix world = Matrix::Identity;
	Matrix view = Matrix::Identity;
	Matrix projection = Matrix::Identity;
};

static_assert((sizeof(CB_Transform) % 16) == 0,
	"Constant Buffer size must be 16-byte aligned");

struct CB_Light
{
	Vector3 direction;
	float pad0;
	Vector4 ambient;
	Vector4 diffuse;
	Vector4 specular;
	Vector3 eyePos;
	float pad1;
};

static_assert((sizeof(CB_Light) % 16) == 0,
	"Constant Buffer size must be 16-byte aligned");

struct CB_Material
{
	Vector4 ambient;
	Vector4 diffuse;
	Vector4 specular;
	float specularPower;
	Vector3 pad0;
};

static_assert((sizeof(CB_Material) % 16) == 0,
	"Constant Buffer size must be 16-byte aligned");

struct CB_UseTextureMap
{
	bool isUseDiffuseMap = false;
	bool pad0[3];
	bool isUseNormalMap = false;
	bool pad1[3];
	bool isUseSpecularMap = false;
	bool pad2[3];
	bool isUseEmissiveMap = false;
	bool pad3[3];
	bool isUseOpacityMap = false;
	bool pad4[3];
	Vector3 pad5;
};

static_assert((sizeof(CB_UseTextureMap) % 16) == 0,
	"Constant Buffer size must be 16-byte aligned");

struct CameraInfo
{
	Vector3 position;
	Vector3 direction;
	Vector3 headDir;
	Matrix matView;

	float fovY;
	float nearZ;
	float farZ;
	Matrix matProjection;
};

static_assert((sizeof(CameraInfo) % 16) == 0,
	"Constant Buffer size must be 16-byte aligned");