#pragma once
#include "GameProcessor.h"
#include "Struct.h"
#include "Model.h"


class FBXTransformAnimation :
	public GameProcessor
{
public:
	FBXTransformAnimation(const int32& width, const int32& height, const std::wstring& name);
	virtual ~FBXTransformAnimation();

	void Initialize() override;
	void Update() override;
	void Render() override;

	void CreateInputLayout() override;
	void CreateConstantBuffer() override;

	void RenderImGUI() override;

private:
	shared_ptr<Model> m_model;

	// Constant Buffer
	ComPtr<ID3D11Buffer> m_pConstantBuffer;
	ComPtr<ID3D11Buffer> m_pBoolBuffer;
	ComPtr<ID3D11Buffer> m_pTransformBuffer;
	ComPtr<ID3D11Buffer> m_pLightBuffer;

	CB_ConstantBuffer m_cbConstant;
	CB_BoolBuffer m_cbBool;
	CB_TransformBuffer m_cbTransform;
	CB_LightDirBuffer m_cbLight;

	Matrix m_world = Matrix::Identity;
	Matrix m_view = Matrix::Identity;
	Matrix m_projection = Matrix::Identity;

	XMVECTOR m_eye;
	XMVECTOR m_at;
	XMVECTOR m_up;

	// imgui 조절 변수
	float m_cameraPos[3] = { 0.0f, 300.0f, -1000.0f };
	float m_ambient = 0.1f;
	float m_specularPower = 4.0f;
	Vector4 m_lightColor = Vector4(1, 1, 1, 1);
	Vector4 m_lightDir = Vector4(0, 0, 1, 1);
	bool m_isUseGamma = true;
	float m_modelScale = 1.f;
	Vector3 m_modelTransform = Vector3(0, -350.f, 500.f);
	Vector2 m_rotation = Vector2(0.0f, 0.0f);
	float m_cameraFOV = 100.f;
	float m_cameraNear = 0.01f;
	float m_cameraFar = 10000.f;
};

