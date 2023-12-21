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
	Matrix m_world = Matrix::Identity;				// 월드좌표계 공간으로 변환을 위한 행렬.
	Matrix m_view = Matrix::Identity;				// 뷰좌표계 공간으로 변환을 위한 행렬.
	Matrix m_projection = Matrix::Identity;			// 단위장치좌표계( Normalized Device Coordinate) 공간으로 변환을 위한 행렬.
	// -> 이후 world 는 오브젝트마다 가지게 하고 view 와 projection 은 카메라가 가지도록 한다.

	// Constant Buffers
	

	// FBX
	vector<shared_ptr<Model>> m_models;		// 가지고 있는 모델들


	// ImGUI 로 조절할 camera 변수 목록
	Vector4 m_cameraPos = Vector4(0.0f, 300.0f, -1000.0f, 0.f);
	float m_cameraFOV = 30.f;
	float m_cameraNear = 0.01f;
	float m_cameraFar = 500.0f;
	Vector2 m_rotation = Vector2(0.0f, 0.0f);
	float m_modelScale = 3.f;


	/// new
	CB_Coordinate m_CBCoordinate;
	ComPtr<ID3D11Buffer> m_CBCoordinateBuffer;

	ComPtr<ID3D11Buffer> m_CBModelTransformBuffer;

	CB_UseTextureMap m_CBUseTextureMap;
	ComPtr<ID3D11Buffer> m_CBUseTextureMapBuffer;

	CB_Camera m_CBCamera;
	ComPtr<ID3D11Buffer> m_CBCameraBuffer;

	CB_Light m_CBLight;
	ComPtr<ID3D11Buffer> m_CBLightBuffer = nullptr;

};

