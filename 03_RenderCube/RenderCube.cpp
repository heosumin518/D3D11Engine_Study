#include "pch.h"
#include "RenderCube.h"

#include "WinApp.h"

RenderCube::RenderCube(const int32& width, const int32& height, const std::wstring& name)
	: GameProcessor(width, height, name)
{

}

RenderCube::~RenderCube()
{
	GameProcessor::UnInitImGUI();
}

void RenderCube::Initialize()
{
	GameProcessor::CreateDeviceAndSwapChain();
	GameProcessor::CreateRenderTargetView();
	GameProcessor::SetViewport();
	GameProcessor::CreateDepthStencilView();

	CreateGeometry();
	GameProcessor::CreateVertexShader();
	GameProcessor::CreateInputLayout();
	GameProcessor::CreatePixelShader();

	GameProcessor::CreateConstantBuffer();

	GameProcessor::InitImGUI();
}

void RenderCube::Update()
{
	GameProcessor::Update();

	auto t = m_timer.TotalTime();

	// set camera
	{
		m_eye = XMVectorSet(m_cameraPos[0], m_cameraPos[1], m_cameraPos[2], 0.0f);
		m_at = XMVectorSet(m_cameraPos[0], m_cameraPos[1] + 1.0f, 100.0f, 0.0f);
		m_up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

		m_view = XMMatrixLookAtLH(m_eye, m_at, m_up);		// ViewTransform 행렬 구하기. XMMatrixLookToLH() 함수로도 구할 수 있음
		m_projection = XMMatrixPerspectiveFovLH(m_cameraFOV / 180.0f * 3.14f , g_winSizeX / static_cast<FLOAT>(g_winSizeY), m_cameraNear, m_cameraFar);		// 0.01f, 100.0f 각각 near 와 far
	}

	// set Sun, Earth, Moon
	{
		// 1st Cube: Rotate around the origin
		Matrix sunSpin = Matrix::CreateRotationY(-t);
		Matrix translation1 = Matrix::CreateTranslation(m_sunPos[0], m_sunPos[1], m_sunPos[2]); //imgui 로 설정
		m_world1 = sunSpin * translation1;

		// 2nd Cube: Rotate around origin
		Matrix earthSpin = Matrix::CreateRotationY(t * 7.0f);
		//Matrix earthTranslate = Matrix::CreateTranslation(-4.0f, 0.0f, 0.0f);
		Matrix earthTranslate = Matrix::CreateTranslation(m_earthPos[0], m_earthPos[1], m_earthPos[2]);
		Matrix earthScale = Matrix::CreateScale(0.3f, 0.3f, 0.3f);
		m_world2 = earthScale * earthSpin * earthTranslate * m_world1;

		// 3rd
		Matrix moonSpin = Matrix::CreateRotationY(-t * 6.0f);
		Matrix moonTranslate = Matrix::CreateTranslation(m_moonPos[0], m_moonPos[1], m_moonPos[2]);
		Matrix moonScale = Matrix::CreateScale(0.3f, 0.3f, 0.3f);
		m_world3 = moonScale * moonSpin * moonTranslate * m_world2;
	}

	// 기존 행렬을 전치행렬로 만들어야 한다. // dx에서의 연산때문.
	// 마지막에 transpose 함수로 전치를 한번만 하자. 월드 마다 하지말고.
	{
		m_transformData1.world = XMMatrixTranspose(m_world1);				
		m_transformData1.view = XMMatrixTranspose(m_view);
		m_transformData1.projection = XMMatrixTranspose(m_projection);

		m_transformData2.world = XMMatrixTranspose(m_world2);				
		m_transformData2.view = XMMatrixTranspose(m_view);
		m_transformData2.projection = XMMatrixTranspose(m_projection);

		m_transformData3.world = XMMatrixTranspose(m_world3);				
		m_transformData3.view = XMMatrixTranspose(m_view);
		m_transformData3.projection = XMMatrixTranspose(m_projection);
	}

}

void RenderCube::Render()
{
	RenderBegin();

	// TO DO


	// Update variables
	//ConstantBuffer constantBuffer;
	//constantBuffer.world = XMMatrixTranspose(m_world);
	//constantBuffer.view = XMMatrixTranspose(m_view);
	//constantBuffer.projection = XMMatrixTranspose(m_projection);
	//m_deviceContext->UpdateSubresource(m_constantBuffer.Get(), 0, nullptr, &constantBuffer, 0, 0);

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
		//  Update Sun, Earth, Moon
		m_deviceContext->UpdateSubresource(m_constantBuffer.Get(), 0, nullptr, &m_transformData1, 0, 0);
		m_deviceContext->DrawIndexed(m_indices.size(), 0, 0);

		m_deviceContext->UpdateSubresource(m_constantBuffer.Get(), 0, nullptr, &m_transformData2, 0, 0);
		m_deviceContext->DrawIndexed(m_indices.size(), 0, 0);

		m_deviceContext->UpdateSubresource(m_constantBuffer.Get(), 0, nullptr, &m_transformData3, 0, 0);
		m_deviceContext->DrawIndexed(m_indices.size(), 0, 0);
	}

	RenderImGUI();

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

void RenderCube::RenderImGUI()
{
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	// camera control window
	{
		ImGui::SetNextWindowPos(ImVec2(525, 10));
		ImGui::SetNextWindowSize(ImVec2(500, 200));		// 메뉴 창 크기 설정
		ImGui::Begin("Camera Control Panel");

		ImGui::Text("Adjust camera position");
		ImGui::SliderFloat3("Camera (x, y, z)", m_cameraPos, -10.0f, 10.0f);
		ImGui::SliderFloat("FOV", &m_cameraFOV, 0.01f, 180.0f);
		ImGui::SliderFloat("Near", &m_cameraNear, 0.01f, 10.0f);
		ImGui::SliderFloat("Far", &m_cameraFar, 1.f, 100.0f);

		ImGui::End();
	}

	// cube control window
	{
		ImGui::SetNextWindowPos(ImVec2(10, 10));
		ImGui::SetNextWindowSize(ImVec2(500, 120));		// 메뉴 창 크기 설정
		ImGui::Begin("Cube Control Panel");

		ImGui::Text("Adjust the planets positions");
		ImGui::SliderFloat3("Sun (x, y, z)", m_sunPos, -5.0f, 5.0f);
		ImGui::SliderFloat3("Earth (x, y, z)", m_earthPos, -5.0f, 5.0f);
		ImGui::SliderFloat3("Moon (x, y, z)", m_moonPos, -5.0f, 5.0f);

		ImGui::End();
	}

	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}

