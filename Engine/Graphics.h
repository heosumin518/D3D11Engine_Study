#pragma once

namespace Engine
{
	class Graphics
	{
		DECLARE_SINGLE(Graphics);

	public:
		void Init(HWND hWnd);

		void RenderBegin();
		void RenderEnd();

		ComPtr<ID3D11Device> GetDevice() { return m_device; }
		ComPtr<ID3D11DeviceContext> GetDeviceContext() { return m_deviceContext; }

	private:
		void CreateDeviceAndSwapChain();
		void CreateRenderTargetView();
		void CreateDepthStencilView();
		void SetViewport();

	private:
		HWND m_hWnd = {};

		// Device & SwapChain
		ComPtr<ID3D11Device> m_device;
		ComPtr<ID3D11DeviceContext> m_deviceContext;
		ComPtr<IDXGISwapChain> m_swapChain;

		// Render Target View
		ComPtr<ID3D11RenderTargetView> m_renderTargetView;

		// Depth Stencil View
		ComPtr<ID3D11Texture2D> m_depthStencilTexture;
		ComPtr<ID3D11DepthStencilView> m_depthStencilView;

		// Misc
		D3D11_VIEWPORT m_viewport = { 0 };
	};
}

