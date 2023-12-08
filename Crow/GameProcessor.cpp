#include "pch.h"
#include "GameProcessor.h"
#include "WinApp.h"


GameProcessor::GameProcessor(const int32 &width, const int32 &height, const wstring &name)
	: m_width(width)
	, m_height(height)
	, m_title(name)
{
	
}

void GameProcessor::Initialize()
{
	// TO DO
	// ���⼭ �Ŵ��� ���� �� �ʱⰪ ����
}

void GameProcessor::Update()
{
	// �ð� ������Ʈ
	m_timer.Tick();

}

void GameProcessor::RenderBegin()
{
	//m_deviceContext->OMSetRenderTargets(1, m_renderTargetView.GetAddressOf(), nullptr);
	m_deviceContext->ClearRenderTargetView(m_renderTargetView.Get(), m_clearColor);		// ������ �������� ȭ�� Ŭ����
	m_deviceContext->ClearDepthStencilView(m_depthStancilView.Get(), D3D11_CLEAR_DEPTH, 1.f, 0);	// �������� 1.0f�� �ʱ�ȭ
	m_deviceContext->OMSetRenderTargets(1, m_renderTargetView.GetAddressOf(), m_depthStancilView.Get());
	m_deviceContext->RSSetViewports(1, &m_viewport);
}

void GameProcessor::RenderEnd()
{
	HR_T(m_swapChain->Present(1, 0));
}

// ��ġ ����, ����ü�� ����, ��ġ ���ؽ�Ʈ ����
void GameProcessor::CreateDeviceAndSwapChain()
{
	// ����ü�� �Ӽ� ���� ����ü ���� �� �ʱ�ȭ
	DXGI_SWAP_CHAIN_DESC swapDesc;
	ZeroMemory(&swapDesc, sizeof(swapDesc));	// swapDesc �� ��� ��������� 0���� �ʱ�ȭ
	{
		// �����(�ؽ�ó)�� ����/���� ũ�� ����.
		swapDesc.BufferDesc.Width = m_width;
		swapDesc.BufferDesc.Height = m_height;

		// ȭ�� �ֻ��� ����
		swapDesc.BufferDesc.RefreshRate.Numerator = 60;
		swapDesc.BufferDesc.RefreshRate.Denominator = 1;

		swapDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		swapDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		swapDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

		// ���ø� ���� ����
		swapDesc.SampleDesc.Count = 1;
		swapDesc.SampleDesc.Quality = 0;

		swapDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		swapDesc.BufferCount = 1;
		swapDesc.OutputWindow = WinApp::GetHandleWnd(); 	// ����ü�� ����� â �ڵ� ��
		swapDesc.Windowed = true;	// â��� ����
		swapDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	}

	UINT creationFlags = 0;

#ifdef _DEBUG
	creationFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	HR_T(::D3D11CreateDeviceAndSwapChain(
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr,
		creationFlags,
		nullptr,
		0,
		D3D11_SDK_VERSION,
		&swapDesc,
		m_swapChain.GetAddressOf(),
		m_device.GetAddressOf(),
		nullptr,
		m_deviceContext.GetAddressOf()
	));
}

// ����۸� �̿��ϴ� ����Ÿ�ٺ� ����
void GameProcessor::CreateRenderTargetView()
{
	ComPtr<ID3D11Texture2D> backBuffer = nullptr;
	HR_T(m_swapChain->GetBuffer( 0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(backBuffer.GetAddressOf())));
	HR_T(m_device->CreateRenderTargetView(backBuffer.Get(), nullptr, m_renderTargetView.GetAddressOf()));
}

void GameProcessor::SetViewport()
{
	m_viewport.TopLeftX = 0.f;
	m_viewport.TopLeftY = 0.f;
	m_viewport.Width = static_cast<float>(m_width);
	m_viewport.Height = static_cast<float>(m_height);
	m_viewport.MinDepth = 0.f;
	m_viewport.MaxDepth = 1.f;
}

void GameProcessor::CreateDepthStencilView()
{
	D3D11_TEXTURE2D_DESC desc;
	ZeroMemory(&desc, sizeof(D3D11_TEXTURE2D_DESC));
	desc.Width = m_width;
	desc.Height = m_height;
	desc.MipLevels = 1;
	desc.ArraySize = 1;
	desc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	desc.CPUAccessFlags = 0;
	desc.MiscFlags = 0;

	ComPtr<ID3D11Texture2D> textureDepthStencil = nullptr;
	HR_T(m_device->CreateTexture2D(&desc, nullptr, textureDepthStencil.GetAddressOf()));

	D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc;
	ZeroMemory(&dsvDesc, sizeof(D3D11_DEPTH_STENCIL_VIEW_DESC));
	dsvDesc.Format = desc.Format;
	dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	dsvDesc.Texture2D.MipSlice = 0;

	HR_T(m_device->CreateDepthStencilView(textureDepthStencil.Get(), &dsvDesc, m_depthStancilView.GetAddressOf()));
}

void GameProcessor::CreateBlendState()
{
	//7. ����ó���� ���� ���� ���� ����
	D3D11_BLEND_DESC blendDesc = {};
	blendDesc.AlphaToCoverageEnable = false;
	blendDesc.IndependentBlendEnable = false;
	blendDesc.RenderTarget[0].BlendEnable = true;
	blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE;
	blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	HR_T(m_device->CreateBlendState(&blendDesc, m_blendState.GetAddressOf()));
}


// Render���� ���������ο� ���ε���  ���ؽ� ���̴� ����
void GameProcessor::CreateVertexShader()
{
	HR_T(CompileShaderFromFile(L"VertexShader.hlsl", "vs_main", "vs_5_0", m_vsBlob));
	HR_T(m_device->CreateVertexShader(m_vsBlob->GetBufferPointer(), m_vsBlob->GetBufferSize(), nullptr, m_vertexShader.GetAddressOf()));
}

// Render���� ���������ο� ���ε��� �ȼ� ���̴� ����
void GameProcessor::CreatePixelShader()
{
	HR_T(CompileShaderFromFile(L"PixelShader.hlsl", "ps_main", "ps_5_0", m_psBlob));
	HR_T(m_device->CreatePixelShader(m_psBlob->GetBufferPointer(), m_psBlob->GetBufferSize(), nullptr, m_pixelShader.GetAddressOf()));
}

void GameProcessor::CreateRasterizeState()
{
	D3D11_RASTERIZER_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	desc.FillMode = D3D11_FILL_SOLID;
	desc.CullMode = D3D11_CULL_BACK;
	desc.FrontCounterClockwise = false;		// ������ �ð�������� ���鼭 �ﰢ���� �׸��� ������ �պκ����� ����. true�� ������ �ݴ������ ���Ѵ�.

	HRESULT hr = m_device->CreateRasterizerState(&desc, m_rasterizerState.GetAddressOf());
	assert(SUCCEEDED(hr));
}

void GameProcessor::CreateSamplerState()
{
	D3D11_SAMPLER_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	desc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	desc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	desc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	desc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	desc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	desc.MaxLOD = 0;
	desc.MinLOD = D3D11_FLOAT32_MAX;

	HR_T(m_device->CreateSamplerState(&desc, m_samplerState.GetAddressOf()));
}

void GameProcessor::InitImGUI()
{
	/*
	ImGUI �ʱ�ȭ.
	*/
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();

	// Setup Dear ImGui style
	//ImGui::StyleColorsLight();
	ImGui::StyleColorsDark();

	// Setup Platform/Renderer backends
	ImGui_ImplWin32_Init(WinApp::GetHandleWnd());
	ImGui_ImplDX11_Init(this->m_device.Get(), this->m_deviceContext.Get());
}

void GameProcessor::UnInitImGUI()
{
	// Cleanup
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}
