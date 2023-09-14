#include "pch.h"
#include "RenderCube.h"

RenderCube::RenderCube(const int32& width, const int32& height, const std::wstring& name)
	: GameProcessor(width, height, name)
{

}

void RenderCube::Initialize()
{
	CreateDeviceAndSwapChain();
	CreateRenderTargetView();
	SetViewport();

	CreateGeometry();
	SetTransformMatrix();
	CreateVertexShader();
	CreateInputLayout();
	CreatePixelShader();

	CreateConstantBuffer();
}

void RenderCube::Update()
{
	GameProcessor::Update();


	// 큐브 상하좌우 회전
	{
		auto t = m_timer.TotalTime();

		XMMATRIX spin = XMMatrixRotationZ(-t);
		XMMATRIX orbit = XMMatrixRotationY(-t * 2.0f);
		//XMMATRIX translate = XMMatrixTranslation(-4.0f, 0.0f, 0.0f);
		XMMATRIX scale = XMMatrixScaling(0.7f, 0.7f, 0.7f);
		m_world = scale * spin * orbit;
	}
	

	//m_world = XMMatrixRotationY(m_timer.TotalTime());
	
}

void RenderCube::Render()
{
	RenderBegin();

	// TO DO

	// Update variables
	ConstantBuffer constantBuffer;
	constantBuffer.world = XMMatrixTranspose(m_world);
	constantBuffer.view = XMMatrixTranspose(m_view);
	constantBuffer.projection = XMMatrixTranspose(m_projection);
	m_deviceContext->UpdateSubresource(m_constantBuffer.Get(), 0, nullptr, &constantBuffer, 0, 0);

	// IA - VS - RS - PS - OM
	{
		uint32 stride = sizeof(Vertex);
		uint32 offset = 0;

		// IA
		m_deviceContext->IASetVertexBuffers(0, 1, m_vertexBuffer.GetAddressOf(), &stride, &offset);
		m_deviceContext->IASetInputLayout(m_inputLayout.Get());
		m_deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		m_deviceContext->IASetIndexBuffer(m_indexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0);	// INDEX값의 범위

		// VS
		m_deviceContext->VSSetShader(m_vertexShader.Get(), nullptr, 0);
		m_deviceContext->VSSetConstantBuffers(0, 1, m_constantBuffer.GetAddressOf());

		// RS

		// PS
		m_deviceContext->PSSetShader(m_pixelShader.Get(), nullptr, 0);

		// OM
		m_deviceContext->DrawIndexed(m_indices.size(), 0, 0);
	}

	RenderEnd();
}

void RenderCube::CreateGeometry()
{
	// Vertex Data 준비
	// Render()에서 파이프라인에 바인딩할 버텍스 정보 준비
	m_vertices.resize(8);

	m_vertices = 
	{
		{ Vector3(-1.0f, 1.0f, -1.0f),	Color(0.0f, 0.0f, 1.0f, 1.0f) },
		{ Vector3(1.0f, 1.0f, -1.0f),	Color(0.0f, 1.0f, 0.0f, 1.0f) },
		{ Vector3(1.0f, 1.0f, 1.0f),	Color(0.0f, 1.0f, 1.0f, 1.0f) },
		{ Vector3(-1.0f, 1.0f, 1.0f),	Color(1.0f, 0.0f, 0.0f, 1.0f) },
		{ Vector3(-1.0f, -1.0f, -1.0f), Color(1.0f, 0.0f, 1.0f, 1.0f) },
		{ Vector3(1.0f, -1.0f, -1.0f),	Color(1.0f, 1.0f, 0.0f, 1.0f) },
		{ Vector3(1.0f, -1.0f, 1.0f),	Color(1.0f, 1.0f, 1.0f, 1.0f) },
		{ Vector3(-1.0f, -1.0f, 1.0f),	Color(0.0f, 0.0f, 0.0f, 1.0f) },
	};

	// Index Data
	// Render()에서 파이프라인에 바인딩할 인덱스 버퍼 생성
	m_indices =
	{
		3,1,0, 2,1,3,
		0,5,4, 1,5,0,
		3,4,7, 0,4,3,
		1,6,5, 2,6,1,
		2,7,6, 3,7,2,
		6,4,5, 7,4,6,
	};

	// 인덱스 개수 저장.
	m_indicesNum = m_indices.size();	// 만들고 안쓰고 있는 애. 지우자.

	// Vertex Buffer, Index Buffer 정보 준비.
	GameProcessor::CreateGeometry();
}

void RenderCube::SetTransformMatrix()
{
	// 쉐이더에 전달할 데이터 설정
	// Initialize the world matrix
	m_world = ::XMMatrixIdentity();

	// Initialize the view matrix
	m_eye = XMVectorSet(0.0f, 1.0f, -5.0f, 0.0f);
	m_at = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	m_up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	m_view = XMMatrixLookAtLH(m_eye, m_at, m_up);		// ViewTransform 행렬 구하기. XMMatrixLookToLH() 함수로도 구할 수 있음

	// Initialize the projection matrix
	m_projection = XMMatrixPerspectiveFovLH(XM_PIDIV2, g_winSizeX / static_cast<FLOAT>(g_winSizeY), 0.01f, 100.0f);
}
