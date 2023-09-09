#include "pch.h"
#include "ScreenClear.h"

ScreenClear::ScreenClear(const int32& width, const int32& height, const std::wstring& name)
	: GameProcessor(width, height, name)
{
	
}

void ScreenClear::Initialize()
{
	CreateDeviceAndSwapChain();
	CreateRenderTargetView();
	SetViewport();
}

void ScreenClear::Update()
{
	GameProcessor::Update();
}

void ScreenClear::Render()
{
	RenderBegin();

	// TO DO
		// IA - VS - RS - PS - OM
	{
		uint32 stride = sizeof(Vertex);
		uint32 offset = 0;
		// IA
		m_deviceContext->IASetVertexBuffers(0, 1, m_vertexBuffer.GetAddressOf(), &stride, &offset);
		m_deviceContext->IASetInputLayout(m_inputLayout.Get());
		m_deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);		// 사각형은 strip으로

		// VS
		m_deviceContext->VSSetShader(m_vertexShader.Get(), nullptr, 0);

		// RS

		// PS
		m_deviceContext->PSSetShader(m_pixelShader.Get(), nullptr, 0);

		// OM
		m_deviceContext->Draw(m_vertices.size(), 0);
	}

	RenderEnd();
}
