#pragma once

class GameProcessor
{
public:
	GameProcessor(const int32 &width, const int32 &height, const wstring &name);
	virtual ~GameProcessor() = default;

	virtual void Initialize() = 0;  // 상속 받은 클래스에서 구현
	virtual void Update();		// 상속 받은 클래스에서 구현
	virtual void Render() = 0;	// 상속 받은 클래스에서 구현

	inline UINT GetWidth() const;
	inline UINT GetHeight() const;
	inline const WCHAR* GetTitle() const;

protected:
	void RenderBegin();
	void RenderEnd();

	void CreateDeviceAndSwapChain();
	void CreateRenderTargetView();
	void SetViewport();
	void CreateDepthStencilView();

	virtual void SetTransformMatrix();

	virtual void CreateGeometry();
	void CreateInputLayout();
	virtual void CreateVertexShader();
	virtual void CreatePixelShader();

	void CreateConstantBuffer();

protected:
	UINT m_width;
	UINT m_height;
	wstring m_title;
	GameTimer m_timer;

	// Device & SwapChain
	ComPtr<ID3D11Device> m_device = nullptr;
	ComPtr<ID3D11DeviceContext> m_deviceContext = nullptr;
	ComPtr<IDXGISwapChain> m_swapChain = nullptr;

	// RTV
	ComPtr<ID3D11RenderTargetView> m_renderTargetView;

	// Misc
	D3D11_VIEWPORT m_viewport = { 0 };
	float m_clearColor[4] = { 0.f, 0.f, 0.f, 1.0f };

	// Geometry
	vector<Vertex> m_vertices;
	ComPtr<ID3D11Buffer> m_vertexBuffer = nullptr;
	ComPtr<ID3D11InputLayout> m_inputLayout = nullptr;
	ComPtr<ID3D11Buffer> m_indexBuffer = nullptr;
	vector<WORD> m_indices;
	int m_indicesNum = 0;	// 인덱스 개수
	// VS
	ComPtr<ID3D11VertexShader> m_vertexShader = nullptr;
	ComPtr<ID3DBlob> m_vsBlob = nullptr;

	// RS
	ComPtr<ID3D11DepthStencilView> m_depthStancilView = nullptr;

	// PS
	ComPtr<ID3D11PixelShader> m_pixelShader = nullptr;
	ComPtr<ID3DBlob> m_psBlob = nullptr;

	// world, view, projection 변환
	Matrix m_world;			// 월드 좌표계 공간으로 변환을 위한 행렬
	Matrix m_view;			// 카메라 좌표계 공간으로 변환을 위한 행렬
	Matrix m_projection;	// 단위장치좌표계(Normalized Device Coordinate) 공간으로 변환을 위한 행렬.

	// Scale Rotation Translate
	TransformData m_transformData;
	ComPtr<ID3D11Buffer> m_constantBuffer = nullptr;

	Vector3 m_localPosition = { 0.f, 0.f, 0.f };
	Vector3 m_localRotation = { 0.f, 0.f, 0.f };
	Vector3 m_localScale = { 1.f, 1.f, 1.f };
	

	// camera
	XMVECTOR m_eye;		// 월드 공간에서의 카메라 좌표(정점)
	XMVECTOR m_at;		// 카메라의 시점에서 바라보는 지점의 좌표(정점)
	XMVECTOR m_up;		// 카메라 상단의 방향을 나타내는 벡터
};

inline UINT GameProcessor::GetWidth() const
{
	return m_width;
}

inline UINT GameProcessor::GetHeight() const
{
	return m_height;
}

inline const WCHAR* GameProcessor::GetTitle() const
{
	return m_title.c_str();
}
