#pragma once
#include "GameProcessor.h"

class TexturedCubeLighting :
	public GameProcessor
{
public:
	TexturedCubeLighting(const int32& width, const int32& height, const std::wstring& name);
	virtual ~TexturedCubeLighting();

	void Initialize() override;
	void Update() override;
	void Render() override;

	void CreateGeometry() override;

	void CreateVertexShader() override;
	void CreatePixelShader() override;

private:
	ComPtr<ID3D11PixelShader> m_pixelShaderSolid = nullptr;		// 픽셀 셰이더 라이트 표시용.

	Matrix                m_world;				// 월드좌표계 공간으로 변환을 위한 행렬.
	Matrix                m_view;				// 뷰좌표계 공간으로 변환을 위한 행렬.
	Matrix                m_projection;			// 단위장치좌표계( Normalized Device Coordinate) 공간으로 변환을 위한 행렬.

	XMFLOAT4 m_lightColors[2] =
	{
		XMFLOAT4(1.f, 1.f, 1.f, 1.0f),
		XMFLOAT4(1.0f, 0.8f, 0.0f, 1.0f)
	};
	XMFLOAT4 m_initialLightDirs[2] =
	{
		XMFLOAT4(-0.577f, 0.577f, -0.577f, 1.0f),
		XMFLOAT4(0.0f, 0.0f, -1.0f, 1.0f),
	};
	XMFLOAT4 m_lightDirsEvaluated[2] = {};		// 계산된 라이트 방향

	// 큐브와 라이트들 정보
	TransformData m_cubeTransformData;
	TransformData m_lightTransformData;

	// ImGUI 로 조절할 변수 목록
	float m_cameraPos[3] = { 0.f, 4.0, -12.f };
	float m_cameraFOV = 90.f;
	float m_cameraNear = 0.01f;
	float m_cameraFar = 100.0f;
};

