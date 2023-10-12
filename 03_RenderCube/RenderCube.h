#pragma once
#include "GameProcessor.h"

struct Vertex
{
	Vector3 position;	// 정점 위치 정보
	Color color;		// 정점 색상 정보
};

struct TransformData
{
	Matrix world = Matrix::Identity;
	Matrix view = Matrix::Identity;
	Matrix projection = Matrix::Identity;
};

class RenderCube:
	public GameProcessor
{
public:
	RenderCube(const int32& width, const int32& height, const std::wstring& name);
	~RenderCube() override;

	void Initialize() override;
	void Update() override;
	void Render() override;

	void CreateGeometry() override;
	void CreateInputLayout() override;
	void CreateConstantBuffer() override;

	void RenderImGUI() override;

private:
	vector<Vertex> m_vertices;
	vector<WORD> m_indices;

	ComPtr<ID3D11Buffer> m_transformBuffer = nullptr;

	// Transform
	TransformData m_transformData1;
	TransformData m_transformData2;
	TransformData m_transformData3;
	Matrix m_world1;
	Matrix m_world2;
	Matrix m_world3;

	// ImGUI 로 조절할 변수 목록
	float m_sunPos[3] = {};
	float m_earthPos[3] = { -4.0f, 0.0f, 0.0f };
	float m_moonPos[3] = { 5.0f, 0.0f, 0.0f };
	float m_cameraPos[3] = { 0.f, 1.0, -10.f };
	float m_cameraFOV = 90.f;
	float m_cameraNear = 0.01f;
	float m_cameraFar = 100.0f;
};

