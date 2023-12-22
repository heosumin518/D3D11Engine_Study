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

	/// new
	CB_Transform m_cbTransform;
	ComPtr<ID3D11Buffer> m_pTransformBuffer;

	CB_Light m_cbLight;
	ComPtr<ID3D11Buffer> m_pLightBuffer;

	CB_Material m_cbMaterial;
	ComPtr<ID3D11Buffer> m_pMaterialBuffer;

	CB_UseTextureMap m_cbUseTextureMap;
	ComPtr<ID3D11Buffer> m_pUseTextureMapBuffer;

	CameraInfo m_camera;
};

