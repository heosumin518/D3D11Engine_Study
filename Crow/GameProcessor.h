#pragma once


class GameProcessor
{
public:
	GameProcessor(const int32 &width, const int32 &height, const wstring &name);
	virtual ~GameProcessor() = default;

	virtual void Initialize() = 0;  // ��� ���� Ŭ�������� ����
	virtual void Update();		// ��� ���� Ŭ�������� ����
	virtual void Render() = 0;	// ��� ���� Ŭ�������� ����

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

	void CreateBlendState();

	virtual void CreateGeometry() {};
	virtual void CreateInputLayout() {};
	virtual void CreateVertexShader();
	virtual void CreatePixelShader();

	void CreateRasterizeState();

	void CreateSamplerState();

	virtual void CreateShaderResourceView() {};

	virtual void CreateConstantBuffer() {};

	// ImGUI
	void InitImGUI();
	virtual void RenderImGUI() {};
	void UnInitImGUI();

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
	//float m_clearColor[4] = { 0.7f, 0.8f, 0.9f, 1.0f };
	float m_clearColor[4] = { 0.1f, 0.1f, 0.1f, 1.0f };

	// Geometry
	ComPtr<ID3D11Buffer> m_vertexBuffer = nullptr;
	ComPtr<ID3D11InputLayout> m_inputLayout = nullptr;
	ComPtr<ID3D11Buffer> m_indexBuffer = nullptr;
	int m_indicesNum = 0;	// �ε��� ����
	// VS
	ComPtr<ID3D11VertexShader> m_vertexShader = nullptr;
	ComPtr<ID3DBlob> m_vsBlob = nullptr;

	// RAS
	ComPtr<ID3D11RasterizerState> m_rasterizerState = nullptr;

	// RS
	ComPtr<ID3D11DepthStencilView> m_depthStancilView = nullptr;
	ComPtr<ID3D11BlendState> m_blendState = nullptr;

	// PS
	ComPtr<ID3D11PixelShader> m_pixelShader = nullptr;
	ComPtr<ID3DBlob> m_psBlob = nullptr;

	// world, view, projection ��ȯ
	Matrix m_world;			// ���� ��ǥ�� �������� ��ȯ�� ���� ���
	Matrix m_view;			// ī�޶� ��ǥ�� �������� ��ȯ�� ���� ���
	Matrix m_projection;	// ������ġ��ǥ��(Normalized Device Coordinate) �������� ��ȯ�� ���� ���.

	// camera
	XMVECTOR m_eye;		// ���� ���������� ī�޶� ��ǥ(����)
	XMVECTOR m_at;		// ī�޶��� �������� �ٶ󺸴� ������ ��ǥ(����)
	XMVECTOR m_up;		// ī�޶� ����� ������ ��Ÿ���� ����

	// Texture
	ComPtr<ID3D11ShaderResourceView> m_shaderReasourceView = nullptr;		// �ؽ�ó ���ҽ� ��
	ComPtr<ID3D11SamplerState> m_samplerState = nullptr;		// ���÷� ����
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
