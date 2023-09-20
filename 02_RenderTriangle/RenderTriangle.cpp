#include "pch.h"
#include "RenderTriangle.h"

RenderTriangle::RenderTriangle(const int32& width, const int32& height, const std::wstring& name)
	: GameProcessor(width, height, name)
{

}

void RenderTriangle::Initialize()
{
	GameProcessor::CreateDeviceAndSwapChain();
	GameProcessor::CreateRenderTargetView();
	GameProcessor::SetViewport();

	CreateGeometry();
	CreateVertexShader();
	GameProcessor::CreateInputLayout();
	CreatePixelShader();
}

void RenderTriangle::Update()
{
	GameProcessor::Update();
}

void RenderTriangle::Render()
{
	RenderBegin();

	// TO DO
	// IA - VS - RS - PS - OM
	{
		uint32 stride = sizeof(Vertex);
		uint32 offset = 0;
		// IA
		m_deviceContext->IASetVertexBuffers(0, 1, m_vertexBuffer.GetAddressOf(), &stride, &offset);
		m_deviceContext->IASetIndexBuffer(m_indexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0);
		m_deviceContext->IASetInputLayout(m_inputLayout.Get());
		m_deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		// VS
		m_deviceContext->VSSetShader(m_vertexShader.Get(), nullptr, 0);

		// RS

		// PS
		m_deviceContext->PSSetShader(m_pixelShader.Get(), nullptr, 0);

		// OM
		//m_deviceContext->Draw(m_vertices.size(), 0);
		m_deviceContext->DrawIndexed(m_indices.size(), 0, 0);
	}

	RenderEnd();
}

void RenderTriangle::CreateGeometry()
{
	// 아직은 VertexShader의 World, View, Projection 변환을 사용하지 않으므로 
	// 직접 Normalized Device Coordinate(좌표계)의 위치로 설정한다.
	//      /---------------------(1,1,1)   z값은 깊이값
	//     /                      / |   
	// (-1,1,0)----------------(1,1,0)        
	//   |         v1           |   |
	//   |        /   `         |   |       중앙이 (0,0,0)  
	//   |       /  +   `       |   |
	//   |     /         `      |   |
	//	 |   v0-----------v2    |  /
	// (-1,-1,0)-------------(1,-1,0)

	// VertexData
	{
		m_vertices.resize(4);

		m_vertices[0].position = Vector3(-0.5f, -0.5f, 0.f);
		m_vertices[0].color = Color(1.0f, 0.f, 0.f, 1.f);

		m_vertices[1].position = Vector3(-0.5f, 0.5f, 0.f);
		m_vertices[1].color = Color(0.f, 1.0f, 0.f, 1.f);

		m_vertices[2].position = Vector3(0.5f, -0.5f, 0.f);
		m_vertices[2].color = Color(0.f, 1.f, 1.0f, 1.f);

		m_vertices[3].position = Vector3(0.5f, 0.5f, 0.f);
		m_vertices[3].color = Color(0.f, 0.f, 1.0f, 1.f);
	}

	// Index
	{
		m_indices = { 0, 1, 3 };	// 삼각형
		//m_indices = { 0, 1, 2, 2, 1, 3 };	// 사각형
	}


	// VertexBuffer
	GameProcessor::CreateGeometry();
}

void RenderTriangle::CreateVertexShader()
{
	HR_T(CompileShaderFromFile(L"BasicVertexShader.hlsl", "VS", "vs_5_0", m_vsBlob));
	HR_T(m_device->CreateVertexShader(m_vsBlob->GetBufferPointer(), m_vsBlob->GetBufferSize(), nullptr, m_vertexShader.GetAddressOf()));
}

void RenderTriangle::CreatePixelShader()
{
	HR_T(CompileShaderFromFile(L"BasicPixelShader.hlsl", "PS", "ps_5_0", m_psBlob));
	HR_T(m_device->CreatePixelShader(m_psBlob->GetBufferPointer(), m_psBlob->GetBufferSize(), nullptr, m_pixelShader.GetAddressOf()));
}
