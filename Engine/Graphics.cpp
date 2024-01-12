#include "pch.h"
#include "Graphics.h"

void Engine::Graphics::Init(HWND hWnd)
{
	m_hWnd = hWnd;

	CreateDeviceAndSwapChain();
	CreateRenderTargetView();
	CreateDepthStencilView();
	SetViewport();
}

void Engine::Graphics::RenderBegin()
{
	m_deviceContext->OMSetRenderTargets(1, m_renderTargetView.GetAddressOf(), m_depthStencilView.Get());
	m_deviceContext->ClearRenderTargetView(m_renderTargetView.Get(), reinterpret_cast<float*>(&GAME->GetGameDesc().clearColor));	// 지정된 색상으로 화면 클리어
	m_deviceContext->ClearDepthStencilView(m_depthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.f, 0);	// 뎁스버퍼 1.0f 로 초기화
	m_deviceContext->RSSetViewports(1, &m_viewport);
}

void Engine::Graphics::RenderEnd()
{
	CHECK(m_swapChain->Present(1, 0));
}

void Engine::Graphics::CreateDeviceAndSwapChain()
{
	// 스왑체인 속성 설정 구조체 생성 및 초기화
	DXGI_SWAP_CHAIN_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	{
		// 백버퍼(텍스처)의 가로/세로 크기 설정.
		desc.BufferDesc.Width = GAME->GetGameDesc().width;
		desc.BufferDesc.Height = GAME->GetGameDesc().height;

		// 화면 주사율 설정
		desc.BufferDesc.RefreshRate.Numerator = 60;
		desc.BufferDesc.RefreshRate.Denominator = 1;

		desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		desc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		desc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

		// 샘플링 관련 설정
		desc.SampleDesc.Count = 1;
		desc.SampleDesc.Quality = 0;

		desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		desc.BufferCount = 1;
		desc.OutputWindow = m_hWnd;		// 스왑체인 출력할 창 핸들 값
		desc.Windowed = TRUE;	// 창모드 여부
		desc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	}

	UINT creationFlags = 0;

#ifdef _DEBUG
	creationFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	CHECK(::D3D11CreateDeviceAndSwapChain(
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr,
		creationFlags,
		nullptr,
		0,
		D3D11_SDK_VERSION,
		&desc,
		m_swapChain.GetAddressOf(),
		m_device.GetAddressOf(),
		nullptr,
		m_deviceContext.GetAddressOf()
	));
}

// 백버퍼를 이용하는 렌더타겟뷰 생성
void Engine::Graphics::CreateRenderTargetView()
{
	ComPtr<ID3D11Texture2D> backBuffer = nullptr;
	CHECK(m_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(backBuffer.GetAddressOf())));
	CHECK(m_device->CreateRenderTargetView(backBuffer.Get(), nullptr, m_renderTargetView.GetAddressOf()));
}

void Engine::Graphics::CreateDepthStencilView()
{
	{
		D3D11_TEXTURE2D_DESC desc = { 0 };
		ZeroMemory(&desc, sizeof(desc));
		desc.Width = static_cast<uint32_t>(GAME->GetGameDesc().width);
		desc.Height = static_cast<uint32_t>(GAME->GetGameDesc().height);
		desc.MipLevels = 1;
		desc.ArraySize = 1;
		desc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		desc.SampleDesc.Count = 1;
		desc.SampleDesc.Quality = 0;
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		desc.CPUAccessFlags = 0;
		desc.MiscFlags = 0;

		CHECK(DEVICE->CreateTexture2D(&desc, nullptr, m_depthStencilTexture.GetAddressOf()));
	}

	{
		D3D11_DEPTH_STENCIL_VIEW_DESC desc;
		ZeroMemory(&desc, sizeof(desc));
		desc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		desc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		desc.Texture2D.MipSlice = 0;

		CHECK(DEVICE->CreateDepthStencilView(m_depthStencilTexture.Get(), &desc, m_depthStencilView.GetAddressOf()));
	}
}


void Engine::Graphics::SetViewport()
{
	m_viewport.TopLeftX = 0.0f;
	m_viewport.TopLeftY = 0.0f;
	m_viewport.Width = GAME->GetGameDesc().width;
	m_viewport.Height = GAME->GetGameDesc().height;
	m_viewport.MinDepth = 0.0f;
	m_viewport.MaxDepth = 1.0f;
}
