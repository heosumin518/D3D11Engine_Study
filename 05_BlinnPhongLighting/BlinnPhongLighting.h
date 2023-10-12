#pragma once
#include "GameProcessor.h"

struct Vertex
{
	Vector3 position;	// 정점 위치 정보
	Vector2 uv;			// 텍스처 좌표
	Vector3 normal;
};

struct CB_Transform
{
	Matrix world = Matrix::Identity;
	Matrix view = Matrix::Identity;
	Matrix projection = Matrix::Identity;
};

struct CB_DirectionLight
{
	Vector4 direction = { 0.f, 0.f, 1.0f, 1.0f };
	Vector4 ambient = { 0.1f, 0.1f, 0.1f, 0.1f };
	Vector4 diffuse = { 1.f, 1.f, 1.f, 1.f };
	Vector4 specular = { 0.6f, 0.6f, 0.6f, 0.6f };
	Vector3 eyePos;
	bool useBlinnPhong = true;
	float specularPower = 1000.f;
	Vector3 m_pad0;		// dummy
};


class BlinnPhongLighting :
	public GameProcessor
{
public:
	BlinnPhongLighting(const int32& width, const int32& height, const std::wstring& name);
	virtual ~BlinnPhongLighting();

	void Initialize() override;
	void Update() override;
	void Render() override;

	void CreateGeometry() override;
	void CreateInputLayout() override;
	void CreateConstantBuffer() override;
	void CreateShaderResourceView() override;

	void RenderImGUI() override;

private:
	vector<Vertex> m_vertices;
	vector<WORD> m_indices;

	Matrix m_world;				// 월드좌표계 공간으로 변환을 위한 행렬.
	Matrix m_view;				// 뷰좌표계 공간으로 변환을 위한 행렬.
	Matrix m_projection;			// 단위장치좌표계( Normalized Device Coordinate) 공간으로 변환을 위한 행렬.

	ComPtr<ID3D11Buffer> m_CBTransformBuffer = nullptr;
	ComPtr<ID3D11Buffer> m_CBLightBuffer = nullptr;

	CB_Transform m_CBCube;
	CB_DirectionLight m_CBLight;

	// ImGUI 로 조절할 camera 변수 목록
	Vector3 m_cameraPos = Vector3(0.0f, 0.0f, -4.0f);
	float m_cameraFOV = 70.f;
	float m_cameraNear = 0.01f;
	float m_cameraFar = 70.0f;
	Vector3 m_cubeRotateInfo = { 0.f, 0.f, 0.f };
};

