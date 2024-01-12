#pragma once
#include "ConstantBuffer.h"

struct CameraDesc
{
	Matrix view = Matrix::Identity;
	Matrix projection = Matrix::Identity;
};

struct TransformDesc
{
	Matrix world = Matrix::Identity;
};

struct LightDesc
{
	Vector3 direction = { 0.0f, -1.0f, 1.0f };
	float pad0 = 0.0f;
	Vector3 radiance = { 1.0f, 1.0f, 1.0f };
	float pad1 = 0.0f;
	Vector3 eyePos = {};
	float pad2 = 0.0f;
};

struct MatrixPaletteDesc
{
	Matrix array[128];
};

struct MaterialDesc
{
	int isUseBaseColorMap = true;	
	int isUseNormalMap = true;
	int isUseSpecularMap = true;
	int isUseEmissiveMap = true;
	int isUseOpacityMap = true;
	int isUseMetalnessMap = true;
	int isUseRoughnessMap = true;
	int isUseIBL = false;
};

static_assert((sizeof(MaterialDesc) % 16) == 0,
	"Constant Buffer size must be 16-byte aligned");

namespace Engine
{
	class Model;	// TODO : temp -> resource manager

	class RenderManager
	{
		DECLARE_SINGLE(RenderManager);

	public:
		void Init();
		void Update();
		void Render();

		// TODO : TEMP. 렌더러 분리 후 모델 렌더 테스트를 위한 임시 함수들
	public:
		// static mesh 와 Skeletal mesh 마다 다르게 생성할 예정
		void CreateVeretexShader();
		void CreatePixelShader();	
		void CreateInputLayout();	// 이건 다를려나..?


	private:
		// Blend State
		ComPtr<ID3D11BlendState> m_blendState;

		// Sampler State
		ComPtr<ID3D11SamplerState> m_samplerState;

		// Rasterizer State
		ComPtr<ID3D11RasterizerState> m_rasterizerState;

		// Constant Buffer
		CameraDesc m_cameraData;
		TransformDesc m_transformData;
		LightDesc m_lightData;
		MaterialDesc m_materialData;
		MatrixPaletteDesc m_matrixPaletteData;
		shared_ptr<ConstantBuffer<CameraDesc>> m_cameraBuffer;
		shared_ptr<ConstantBuffer<TransformDesc>> m_transformBuffer;
		shared_ptr<ConstantBuffer<LightDesc>> m_lightBuffer;
		shared_ptr<ConstantBuffer<MaterialDesc>> m_materialBuffer;
		shared_ptr<ConstantBuffer<MatrixPaletteDesc>> m_matrixPaletteBuffer;

		Matrix m_world = Matrix::Identity;
		Matrix m_view = Matrix::Identity;
		Matrix m_projection = Matrix::Identity;

		//TODO : 여기 아래부턴 점점 고쳐나갈 변수들
	private:
		// FBX
		shared_ptr<Model> m_model;		// 가지고 있는 모델

		// ImGUI 로 조절할 camera 변수 목록
		Vector3 m_cameraPos = Vector3(50.0f, 0.0f, -100.0f);
		Vector3 m_rotation = Vector3(0.0f, 0.0f, 82.0f);
		float m_modelScale = 1.f;

		// VS
		ComPtr<ID3D11VertexShader> m_vertexShader = nullptr;
		ComPtr<ID3DBlob> m_vsBlob = nullptr;

		// PS
		ComPtr<ID3D11PixelShader> m_pixelShader = nullptr;
		ComPtr<ID3DBlob> m_psBlob = nullptr;

		// Geometry
		ComPtr<ID3D11InputLayout> m_inputLayout = nullptr;

	};
}
