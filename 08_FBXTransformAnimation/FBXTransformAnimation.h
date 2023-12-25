#pragma once
#include "GameProcessor.h"
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

	Matrix m_world = Matrix::Identity;
	Matrix m_view = Matrix::Identity;
	Matrix m_projection = Matrix::Identity;

	XMVECTOR m_eye;
	XMVECTOR m_at;
	XMVECTOR m_up;

	Vector3 m_cameraPos = Vector3(0.0f, 300.0f, -1000.0f);
	float m_modelScale = 3.f;
	Vector2 m_rotation = Vector2(0.0f, 0.0f);

};

