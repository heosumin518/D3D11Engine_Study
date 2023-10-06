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

	GameProcessor::CreateSamplerState();
	GameProcessor::CreateShaderResourceView();

	GameProcessor::CreateConstantBuffer();

	GameProcessor::InitImGUI();
}

void TexturedCubeLighting::Update()
{
	GameProcessor::Update();

	auto t = m_timer.TotalTime();

	// update camera
	{
		m_eye = XMVectorSet(m_cameraPos[0], m_cameraPos[1], m_cameraPos[2], 0.0f);
		m_at = XMVectorSet(m_cameraPos[0], m_cameraPos[1] + 1.0f, 100.0f, 0.0f);
		m_up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

		m_view = XMMatrixLookAtLH(m_eye, m_at, m_up);		// ViewTransform 행렬 구하기. XMMatrixLookToLH() 함수로도 구할 수 있음
		m_projection = XMMatrixPerspectiveFovLH(m_cameraFOV / 180.0f * 3.14f, g_winSizeX / static_cast<FLOAT>(g_winSizeY), m_cameraNear, m_cameraFar);		// 0.01f, 100.0f 각각 near 와 far
	}

	// update cube
	{
		Matrix spinX = Matrix::CreateRotationX(m_cubeRotateInfo.x);
		Matrix spinY = Matrix::CreateRotationY(m_cubeRotateInfo.y);
		Matrix spinZ = Matrix::CreateRotationZ(m_cubeRotateInfo.z);
		m_world = spinY * spinX * spinZ;

		m_transformData.world = XMMatrixTranspose(m_world);
		m_transformData.view = XMMatrixTranspose(m_view);
		m_transformData.projection = XMMatrixTranspose(m_projection);
	}

	// update light
	{
		m_transformData.lightDir = m_lightDir;
		m_transformData.lightColor = m_lightColor;
	}
}

void TexturedCubeLighting::Render()
{
	RenderBegin();

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
		m_deviceContext->PSSetShaderResources(0, 1, m_shaderReasourceView.GetAddressOf());
		m_deviceContext->PSSetSamplers(0, 1, m_samplerState.GetAddressOf());
		m_deviceContext->PSSetConstantBuffers(0, 1, m_constantBuffer.GetAddressOf());

		// OM
		// Render cube and Light
		m_deviceContext->UpdateSubresource(m_constantBuffer.Get(), 0, nullptr, &m_transformData, 0, 0);
		m_deviceContext->DrawIndexed(m_indices.size(), 0, 0);
	}

	RenderImGUI();

	RenderEnd();
}

void TexturedCubeLighting::RenderImGUI()
{
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	// camera control window
	{
		ImGui::SetNextWindowPos(ImVec2(420, 10));
		ImGui::SetNextWindowSize(ImVec2(450, 150));		// 메뉴 창 크기 설정
		ImGui::Begin("Camera Control Panel");

		ImGui::Text("Adjust camera position");
		ImGui::SliderFloat3("Camera (x, y, z)", m_cameraPos, -10.0f, 10.0f);
		ImGui::SliderFloat("FOV", &m_cameraFOV, 0.01f, 180.0f);
		ImGui::SliderFloat("Near", &m_cameraNear, 0.01f, 10.0f);
		ImGui::SliderFloat("Far", &m_cameraFar, 1.f, 100.0f);

		ImGui::End();
	}

	//ImGui::SliderFloat3("Sun (x, y, z)", m_sunPos, -5.0f, 5.0f);

	// cube control window
	{
		ImGui::SetNextWindowPos(ImVec2(10, 10));
		ImGui::SetNextWindowSize(ImVec2(400, 120));		// 메뉴 창 크기 설정
		ImGui::Begin("Cube Control Panel");

		ImGui::Text("Rotate Cube");
		ImGui::SliderFloat("X", &m_cubeRotateInfo.x, 0.f, 5.0f);
		ImGui::SliderFloat("Y", &m_cubeRotateInfo.y, 0.f, 5.0f);
		ImGui::SliderFloat("Z", &m_cubeRotateInfo.z, 0.f, 5.0f);

		ImGui::End();
	}

	// light control window
	{
		ImGui::SetNextWindowPos(ImVec2(10, 140));
		ImGui::SetNextWindowSize(ImVec2(400, 120));		// 메뉴 창 크기 설정
		ImGui::Begin("Light Control Panel");

		ImGui::Text("Adjust the planets positions");
		ImGui::SliderFloat3("Direction", reinterpret_cast<float*>(&m_lightDir), -1.f, 1.f);
		ImGui::SliderFloat3("Color", reinterpret_cast<float*>(&m_lightColor), 0.f, 1.f);
		ImGui::End();
	}

	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}

void TexturedCubeLighting::CreateGeometry()
{
	// Vertex Data 준비
	// Render()에서 파이프라인에 바인딩할 버텍스 정보 준비
	m_vertices.resize(24);

	m_vertices[0].position = Vector3(-1.0f, 1.0f, -1.0f);	m_vertices[0].normal = Vector3(0.0f, 1.0f, 0.0f); m_vertices[0].uv = Vector2(1.0f, 0.0f);
	m_vertices[1].position = Vector3(1.0f, 1.0f, -1.0f);	m_vertices[1].normal = Vector3(0.0f, 1.0f, 0.0f); m_vertices[1].uv = Vector2(0.0f, 0.0f);
	m_vertices[2].position = Vector3(1.0f, 1.0f, 1.0f);		m_vertices[2].normal = Vector3(0.0f, 1.0f, 0.0f); m_vertices[2].uv = Vector2(0.0f, 1.0f);
	m_vertices[3].position = Vector3(-1.0f, 1.0f, 1.0f);	m_vertices[3].normal = Vector3(0.0f, 1.0f, 0.0f); m_vertices[3].uv = Vector2(1.0f, 1.0f);

	m_vertices[4].position = Vector3(-1.0f, -1.0f, -1.0f);	m_vertices[4].normal = Vector3(0.0f, -1.0f, 0.0f); m_vertices[4].uv = Vector2(0.0f, 0.0f);
	m_vertices[5].position = Vector3(1.0f, -1.0f, -1.0f); 	m_vertices[5].normal = Vector3(0.0f, -1.0f, 0.0f); m_vertices[5].uv = Vector2(1.0f, 0.0f);
	m_vertices[6].position = Vector3(1.0f, -1.0f, 1.0f);  	m_vertices[6].normal = Vector3(0.0f, -1.0f, 0.0f); m_vertices[6].uv = Vector2(1.0f, 1.0f);
	m_vertices[7].position = Vector3(-1.0f, -1.0f, 1.0f); 	m_vertices[7].normal = Vector3(0.0f, -1.0f, 0.0f); m_vertices[7].uv = Vector2(0.0f, 1.0f);

	m_vertices[8].position = Vector3(-1.0f, -1.0f, 1.0f); 	m_vertices[8].normal = Vector3(-1.0f, 0.0f, 0.0f); m_vertices[8].uv = Vector2(0.0f, 1.0f);
	m_vertices[9].position = Vector3(-1.0f, -1.0f, -1.0f);	m_vertices[9].normal = Vector3(-1.0f, 0.0f, 0.0f); m_vertices[9].uv = Vector2(1.0f, 1.0f);
	m_vertices[10].position = Vector3(-1.0f, 1.0f, -1.0f); 	m_vertices[10].normal = Vector3(-1.0f, 0.0f, 0.0f); m_vertices[10].uv = Vector2(1.0f, 0.0f);
	m_vertices[11].position = Vector3(-1.0f, 1.0f, 1.0f);  	m_vertices[11].normal = Vector3(-1.0f, 0.0f, 0.0f); m_vertices[11].uv = Vector2(0.0f, 0.0f);

	m_vertices[12].position = Vector3(1.0f, -1.0f, 1.0f); 	m_vertices[12].normal = Vector3(1.0f, 0.0f, 0.0f); m_vertices[12].uv = Vector2(1.0f, 1.0f);
	m_vertices[13].position = Vector3(1.0f, -1.0f, -1.0f);	m_vertices[13].normal = Vector3(1.0f, 0.0f, 0.0f);  m_vertices[13].uv = Vector2(0.0f, 1.0f);
	m_vertices[14].position = Vector3(1.0f, 1.0f, -1.0f); 	m_vertices[14].normal = Vector3(1.0f, 0.0f, 0.0f);  m_vertices[14].uv = Vector2(0.0f, 0.0f);
	m_vertices[15].position = Vector3(1.0f, 1.0f, 1.0f);  	m_vertices[15].normal = Vector3(1.0f, 0.0f, 0.0f);  m_vertices[15].uv = Vector2(1.0f, 0.0f);

	m_vertices[16].position = Vector3(-1.0f, -1.0f, -1.0f);	m_vertices[16].normal = Vector3(0.0f, 0.0f, -1.0f); m_vertices[16].uv = Vector2(0.0f, 1.0f);
	m_vertices[17].position = Vector3(1.0f, -1.0f, -1.0f); 	m_vertices[17].normal = Vector3(0.0f, 0.0f, -1.0f); m_vertices[17].uv = Vector2(1.0f, 1.0f);
	m_vertices[18].position = Vector3(1.0f, 1.0f, -1.0f);  	m_vertices[18].normal = Vector3(0.0f, 0.0f, -1.0f); m_vertices[18].uv = Vector2(1.0f, 0.0f);
	m_vertices[19].position = Vector3(-1.0f, 1.0f, -1.0f); 	m_vertices[19].normal = Vector3(0.0f, 0.0f, -1.0f); m_vertices[19].uv = Vector2(0.0f, 0.0f);

	m_vertices[20].position = Vector3(-1.0f, -1.0f, 1.0f);	m_vertices[20].normal = Vector3(0.0f, 0.0f, 1.0f); m_vertices[20].uv = Vector2(1.0f, 1.0f);
	m_vertices[21].position = Vector3(1.0f, -1.0f, 1.0f); 	m_vertices[21].normal = Vector3(0.0f, 0.0f, 1.0f); m_vertices[21].uv = Vector2(0.0f, 1.0f);
	m_vertices[22].position = Vector3(1.0f, 1.0f, 1.0f);  	m_vertices[22].normal = Vector3(0.0f, 0.0f, 1.0f); m_vertices[22].uv = Vector2(0.0f, 0.0f);
	m_vertices[23].position = Vector3(-1.0f, 1.0f, 1.0f); 	m_vertices[23].normal = Vector3(0.0f, 0.0f, 1.0f); m_vertices[23].uv = Vector2(1.0f, 0.0f);

	{
		//m_vertices =
		//{
		//	{ Vector3(-1.0f, 1.0f, -1.0f), Vector3(0.0f, 1.0f, 0.0f),  Vector2(1.0f, 0.0f) },
		//	{ Vector3(1.0f, 1.0f, -1.0f),  Vector3(0.0f, 1.0f, 0.0f),  Vector2(0.0f, 0.0f) },
		//	{ Vector3(1.0f, 1.0f, 1.0f),   Vector3(0.0f, 1.0f, 0.0f),  Vector2(0.0f, 1.0f) },
		//	{ Vector3(-1.0f, 1.0f, 1.0f),  Vector3(0.0f, 1.0f, 0.0f),  Vector2(1.0f, 1.0f) },

		//	{ Vector3(-1.0f, -1.0f, -1.0f),Vector3(0.0f, -1.0f, 0.0f), Vector2(0.0f, 0.0f) },
		//	{ Vector3(1.0f, -1.0f, -1.0f), Vector3(0.0f, -1.0f, 0.0f), Vector2(1.0f, 0.0f) },
		//	{ Vector3(1.0f, -1.0f, 1.0f),  Vector3(0.0f, -1.0f, 0.0f), Vector2(1.0f, 1.0f) },
		//	{ Vector3(-1.0f, -1.0f, 1.0f), Vector3(0.0f, -1.0f, 0.0f), Vector2(0.0f, 1.0f) },

		//	{ Vector3(-1.0f, -1.0f, 1.0f), Vector3(-1.0f, 0.0f, 0.0f), Vector2(0.0f, 1.0f) },
		//	{ Vector3(-1.0f, -1.0f, -1.0f),Vector3(-1.0f, 0.0f, 0.0f), Vector2(1.0f, 1.0f) },
		//	{ Vector3(-1.0f, 1.0f, -1.0f), Vector3(-1.0f, 0.0f, 0.0f), Vector2(1.0f, 0.0f) },
		//	{ Vector3(-1.0f, 1.0f, 1.0f),  Vector3(-1.0f, 0.0f, 0.0f), Vector2(0.0f, 0.0f) },

		//	{ Vector3(1.0f, -1.0f, 1.0f),  Vector3(1.0f, 0.0f, 0.0f),  Vector2(1.0f, 1.0f) },
		//	{ Vector3(1.0f, -1.0f, -1.0f), Vector3(1.0f, 0.0f, 0.0f),  Vector2(0.0f, 1.0f) },
		//	{ Vector3(1.0f, 1.0f, -1.0f),  Vector3(1.0f, 0.0f, 0.0f),  Vector2(0.0f, 0.0f) },
		//	{ Vector3(1.0f, 1.0f, 1.0f),   Vector3(1.0f, 0.0f, 0.0f),  Vector2(1.0f, 0.0f) },

		//	{ Vector3(-1.0f, -1.0f, -1.0f),Vector3(0.0f, 0.0f, -1.0f), Vector2(0.0f, 1.0f) },
		//	{ Vector3(1.0f, -1.0f, -1.0f), Vector3(0.0f, 0.0f, -1.0f), Vector2(1.0f, 1.0f) },
		//	{ Vector3(1.0f, 1.0f, -1.0f),  Vector3(0.0f, 0.0f, -1.0f), Vector2(1.0f, 0.0f) },
		//	{ Vector3(-1.0f, 1.0f, -1.0f), Vector3(0.0f, 0.0f, -1.0f), Vector2(0.0f, 0.0f) },

		//	{ Vector3(-1.0f, -1.0f, 1.0f), Vector3(0.0f, 0.0f, 1.0f),  Vector2(1.0f, 1.0f) },
		//	{ Vector3(1.0f, -1.0f, 1.0f),  Vector3(0.0f, 0.0f, 1.0f),  Vector2(0.0f, 1.0f) },
		//	{ Vector3(1.0f, 1.0f, 1.0f),   Vector3(0.0f, 0.0f, 1.0f),  Vector2(0.0f, 0.0f) },
		//	{ Vector3(-1.0f, 1.0f, 1.0f),  Vector3(0.0f, 0.0f, 1.0f),  Vector2(1.0f, 0.0f) },
		//};
	}


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
}
