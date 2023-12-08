#pragma once
#include "GameProcessor.h"
#include "Model.h"

struct CB_CoordinateData
{
	Matrix World = Matrix::Identity;
	Matrix View = Matrix::Identity;
	Matrix Projection = Matrix::Identity;
	//Vector4 MeshColor;
};

static_assert((sizeof(CB_CoordinateData) % 16) == 0,
	"Constant Buffer size must be 16-byte aligned");

struct CB_ModelTransform
{
	Matrix World = Matrix::Identity;
};

static_assert((sizeof(CB_ModelTransform) % 16) == 0,
	"Constant Buffer size must be 16-byte aligned");

struct CB_DirectionLight
{
	Vector4 Direction;
	Vector4 LightColor;
	float SpecularPower;;
	Vector3 AmbientColor;
};

static_assert((sizeof(CB_DirectionLight) % 16) == 0,
	"Constant Buffer size must be 16-byte aligned");

struct CB_Camera
{
	Vector4 position;
};

static_assert((sizeof(CB_Camera) % 16) == 0,
	"Constant Buffer size must be 16-byte aligned");

struct CB_UseTextureMap
{
	int UseNormalMap;
	int UseSpecularMap;
	int UseGammaCorrection;
	int dummy;
};

static_assert((sizeof(CB_UseTextureMap) % 16) == 0,
	"Constant Buffer size must be 16-byte aligned");

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
	Matrix m_world = Matrix::Identity;				// 월드좌표계 공간으로 변환을 위한 행렬.
	Matrix m_view = Matrix::Identity;				// 뷰좌표계 공간으로 변환을 위한 행렬.
	Matrix m_projection = Matrix::Identity;			// 단위장치좌표계( Normalized Device Coordinate) 공간으로 변환을 위한 행렬.
	// -> 이후 world 는 오브젝트마다 가지게 하고 view 와 projection 은 카메라가 가지도록 한다.

	// Constant Buffers
	ComPtr<ID3D11Buffer> m_CBCoordinateBuffer;
	ComPtr<ID3D11Buffer> m_CBLightBuffer;
	ComPtr<ID3D11Buffer> m_CBCameraBuffer;
	ComPtr<ID3D11Buffer> m_CBTextureBuffer;
	ComPtr<ID3D11Buffer> m_CBModelBuffer;

	//CB_Transform m_CBModel;
	CB_CoordinateData m_CBCoordinate;
	CB_DirectionLight m_CBLight;
	CB_Camera m_CBCamera;
	CB_UseTextureMap m_CBUseTextureMap;

	// FBX
	vector<shared_ptr<Model>> m_models;		// 가지고 있는 모델들


	// ImGUI 로 조절할 camera 변수 목록
	Vector4 m_cameraPos = Vector4(0.0f, 300.0f, -1000.0f, 0.f);
	float m_cameraFOV = 30.f;
	float m_cameraNear = 0.01f;
	float m_cameraFar = 500.0f;
	Vector2 m_rotation = Vector2(0.0f, 0.0f);
	float m_modelScale = 3.f;

	Vector4 m_directionLightDir = { 0.f, 0.f, 1.f, 0.0f };
	Vector4 m_DirectionLightColor = { 0.5f, 0.5f, 0.5f, 1.f };
	Vector3 m_ambientColor = { 0.1f, 0.1f, 0.1f };
	float m_specularPower = 500.f;

	bool m_isNormalMap = true;
	bool m_isSpecularMap = true;
	bool m_isGammaCorrection = true;
};

