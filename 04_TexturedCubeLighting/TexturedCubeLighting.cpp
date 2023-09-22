#include "pch.h"
#include "TexturedCubeLighting.h"

TexturedCubeLighting::TexturedCubeLighting(const int32& width, const int32& height, const std::wstring& name)
	: GameProcessor(width, height, name)
{

}

TexturedCubeLighting::~TexturedCubeLighting()
{
	GameProcessor::UnInitImGUI();
}

void TexturedCubeLighting::Initialize()
{
	GameProcessor::CreateDeviceAndSwapChain();
	GameProcessor::CreateRenderTargetView();
	GameProcessor::SetViewport();
	GameProcessor::CreateDepthStencilView();

	CreateGeometry();
	CreateVertexShader();
	GameProcessor::CreateInputLayout();
	CreatePixelShader();

	GameProcessor::CreateConstantBuffer();

	GameProcessor::InitImGUI();

	// 초기값설정
	m_world = XMMatrixIdentity();
	XMVECTOR Eye = XMVectorSet(0.0f, 4.0f, -12.0f, 0.0f);
	XMVECTOR At = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	XMVECTOR Up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	m_view = XMMatrixLookAtLH(Eye, At, Up);
	m_projection = XMMatrixPerspectiveFovLH(XM_PIDIV4, g_winSizeX / (FLOAT)g_winSizeY, 0.01f, 100.0f);
}

void TexturedCubeLighting::Update()
{
	GameProcessor::Update();

	auto t = m_timer.TotalTime();

	//// update camera
	//{
	//	m_eye = XMVectorSet(m_cameraPos[0], m_cameraPos[1], m_cameraPos[2], 0.0f);
	//	m_at = XMVectorSet(m_cameraPos[0], m_cameraPos[1] + 1.0f, 100.0f, 0.0f);
	//	m_up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

	//	m_view = XMMatrixLookAtLH(m_eye, m_at, m_up);		// ViewTransform 행렬 구하기. XMMatrixLookToLH() 함수로도 구할 수 있음
	//	m_projection = XMMatrixPerspectiveFovLH(m_cameraFOV / 180.0f * 3.14f, g_winSizeX / static_cast<FLOAT>(g_winSizeY), m_cameraNear, m_cameraFar);		// 0.01f, 100.0f 각각 near 와 far
	//}

	m_world = Matrix::Identity;
	//m_world = Matrix::CreateRotationY(t);
	m_world = Matrix::CreateRotationY(59.225f);		// TODO

	m_lightDirsEvaluated[0] = m_initialLightDirs[0];

	// Rotate the second light around the origin
	Matrix rotate = Matrix::CreateRotationY(-2.0f * t);
	XMVECTOR lightDir = XMLoadFloat4(&m_initialLightDirs[1]);
	lightDir = XMVector3Transform(lightDir, rotate);
	XMStoreFloat4(&m_lightDirsEvaluated[1], lightDir);
}

void TexturedCubeLighting::Render()
{
	RenderBegin();

	//// Update cube variables
	//m_cubeTransformData.world = XMMatrixTranspose(m_world);
	//m_cubeTransformData.view = XMMatrixTranspose(m_view);
	//m_cubeTransformData.projection = XMMatrixTranspose(m_projection);

	//// Update lighting variables
	//m_lightTransformData.world = XMMatrixTranspose(m_world);
	//m_lightTransformData.view = XMMatrixTranspose(m_view);
	//m_lightTransformData.projection = XMMatrixTranspose(m_projection);
	//m_lightTransformData.lightDir[0] = m_lightDirsEvaluated[0];
	//m_lightTransformData.lightDir[1] = m_lightDirsEvaluated[1];
	//m_lightTransformData.lightColor[0] = m_lightColors[0];
	//m_lightTransformData.lightColor[1] = m_lightColors[1];
	//m_lightTransformData.outputColor = XMFLOAT4(0, 0, 0, 0);

	TransformData cb;
	cb.world = XMMatrixTranspose(m_world);
	cb.view = XMMatrixTranspose(m_view);
	cb.projection = XMMatrixTranspose(m_projection);
	cb.lightDir[0] = m_lightDirsEvaluated[0];
	cb.lightDir[1] = m_lightDirsEvaluated[1];
	cb.lightColor[0] = m_lightColors[0];
	cb.lightColor[1] = m_lightColors[1];
	cb.outputColor = XMFLOAT4(0, 0, 0, 0);
	m_deviceContext->UpdateSubresource(m_constantBuffer.Get(), 0, nullptr, &cb, 0, 0);

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
		m_deviceContext->PSSetConstantBuffers(0, 1, m_constantBuffer.GetAddressOf());

		// OM
		// Render cube
		m_deviceContext->UpdateSubresource(m_constantBuffer.Get(), 0, nullptr, &cb, 0, 0);
		m_deviceContext->DrawIndexed(m_indices.size(), 0, 0);
	}
	// Render each light
	for (int m = 0; m < 2; m++)
	{
		XMMATRIX light = XMMatrixTranslationFromVector(5.0f * XMLoadFloat4(&m_lightDirsEvaluated[m]));
		XMMATRIX lightScale = XMMatrixScaling(0.2f, 0.2f, 0.2f);
		light = lightScale * light;

		// Update the world variable to reflect the current light
		//m_lightTransformData.world = XMMatrixTranspose(light);
		//m_lightTransformData.outputColor = m_lightColors[m];
		cb.world = XMMatrixTranspose(light);
		cb.outputColor = m_lightColors[m];

		m_deviceContext->UpdateSubresource(m_constantBuffer.Get(), 0, nullptr, &cb, 0, 0);

		m_deviceContext->PSSetShader(m_pixelShaderSolid.Get(), nullptr, 0);
		m_deviceContext->DrawIndexed(m_indices.size(), 0, 0);
	}

	RenderEnd();
}

void TexturedCubeLighting::CreateGeometry()
{
	// Vertex Data 준비
	// Render()에서 파이프라인에 바인딩할 버텍스 정보 준비
	m_vertices.resize(24);

	m_vertices =
	{
		{ Vector3(-1.0f, 1.0f, -1.0f),	Vector3(0.0f, 1.0f, 0.0f) },// Normal Y +	
		{ Vector3(1.0f, 1.0f, -1.0f),	Vector3(0.0f, 1.0f, 0.0f) },
		{ Vector3(1.0f, 1.0f, 1.0f),	Vector3(0.0f, 1.0f, 0.0f) },
		{ Vector3(-1.0f, 1.0f, 1.0f),	Vector3(0.0f, 1.0f, 0.0f) },

		{ Vector3(-1.0f, -1.0f, -1.0f), Vector3(0.0f, -1.0f, 0.0f) },// Normal Y -
		{ Vector3(1.0f, -1.0f, -1.0f),	Vector3(0.0f, -1.0f, 0.0f) },
		{ Vector3(1.0f, -1.0f, 1.0f),	Vector3(0.0f, -1.0f, 0.0f) },
		{ Vector3(-1.0f, -1.0f, 1.0f),	Vector3(0.0f, -1.0f, 0.0f) },

		{ Vector3(-1.0f, -1.0f, 1.0f),	Vector3(-1.0f, 0.0f, 0.0f) },//	Normal X -
		{ Vector3(-1.0f, -1.0f, -1.0f), Vector3(-1.0f, 0.0f, 0.0f) },
		{ Vector3(-1.0f, 1.0f, -1.0f),	Vector3(-1.0f, 0.0f, 0.0f) },
		{ Vector3(-1.0f, 1.0f, 1.0f),	Vector3(-1.0f, 0.0f, 0.0f) },

		{ Vector3(1.0f, -1.0f, 1.0f),	Vector3(1.0f, 0.0f, 0.0f) },// Normal X +
		{ Vector3(1.0f, -1.0f, -1.0f),	Vector3(1.0f, 0.0f, 0.0f) },
		{ Vector3(1.0f, 1.0f, -1.0f),	Vector3(1.0f, 0.0f, 0.0f) },
		{ Vector3(1.0f, 1.0f, 1.0f),	Vector3(1.0f, 0.0f, 0.0f) },

		{ Vector3(-1.0f, -1.0f, -1.0f), Vector3(0.0f, 0.0f, -1.0f) }, // Normal Z -
		{ Vector3(1.0f, -1.0f, -1.0f),	Vector3(0.0f, 0.0f, -1.0f) },
		{ Vector3(1.0f, 1.0f, -1.0f),	Vector3(0.0f, 0.0f, -1.0f) },
		{ Vector3(-1.0f, 1.0f, -1.0f),	Vector3(0.0f, 0.0f, -1.0f) },

		{ Vector3(-1.0f, -1.0f, 1.0f),	Vector3(0.0f, 0.0f, 1.0f) },// Normal Z +
		{ Vector3(1.0f, -1.0f, 1.0f),	Vector3(0.0f, 0.0f, 1.0f) },
		{ Vector3(1.0f, 1.0f, 1.0f),	Vector3(0.0f, 0.0f, 1.0f) },
		{ Vector3(-1.0f, 1.0f, 1.0f),	Vector3(0.0f, 0.0f, 1.0f) },
	};

	// Index Data
	// Render() 에서 파이프라인에 바인딩할 인덱스 버퍼 생성
	m_indices =
	{
		3,1,0, 2,1,3,
		6,4,5, 7,4,6,
		11,9,8, 10,9,11,
		14,12,13, 15,12,14,
		19,17,16, 18,17,19,
		22,20,21, 23,20,22
	};


	GameProcessor::CreateGeometry();
}

void TexturedCubeLighting::CreateVertexShader()
{
	HR_T(CompileShaderFromFile(L"Fourth_BasicVertexShader.hlsl", "main", "vs_5_0", m_vsBlob));
	HR_T(m_device->CreateVertexShader(m_vsBlob->GetBufferPointer(), m_vsBlob->GetBufferSize(), nullptr, m_vertexShader.GetAddressOf()));
}

void TexturedCubeLighting::CreatePixelShader()
{
	HR_T(CompileShaderFromFile(L"Fourth_BasicPixelShader.hlsl", "main", "ps_5_0", m_psBlob));
	HR_T(m_device->CreatePixelShader(m_psBlob->GetBufferPointer(), m_psBlob->GetBufferSize(), nullptr, m_pixelShader.GetAddressOf()));

	HR_T(CompileShaderFromFile(L"Fourth_SolidPixelShader.hlsl", "main", "ps_5_0", m_psBlob));
	HR_T(m_device->CreatePixelShader(m_psBlob->GetBufferPointer(), m_psBlob->GetBufferSize(), nullptr, m_pixelShaderSolid.GetAddressOf()));
}
