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
	CreateInputLayout();
	GameProcessor::CreatePixelShader();

	CreateConstantBuffer();

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
		m_projection = XMMatrixPerspectiveFovLH(m_cameraFOV / 180.0f * 3.14f , g_winSizeX / static_cast<FLOAT>(g_winSizeY), m_cameraNear, m_cameraFar);
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
		m_deviceContext->VSSetConstantBuffers(0, 1, m_transformBuffer.GetAddressOf());

		// RS

		// PS
		m_deviceContext->PSSetShader(m_pixelShader.Get(), nullptr, 0);

		// OM
		// Render Sun, Earth, Moon
		m_deviceContext->UpdateSubresource(m_transformBuffer.Get(), 0, nullptr, &m_transformData1, 0, 0);
		m_deviceContext->DrawIndexed(m_indices.size(), 0, 0);

		m_deviceContext->UpdateSubresource(m_transformBuffer.Get(), 0, nullptr, &m_transformData2, 0, 0);
		m_deviceContext->DrawIndexed(m_indices.size(), 0, 0);

		m_deviceContext->UpdateSubresource(m_transformBuffer.Get(), 0, nullptr, &m_transformData3, 0, 0);
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
	// VertexBuffer
	{
		// 정점 버퍼 정보 설정
		D3D11_BUFFER_DESC vbDesc;
		ZeroMemory(&vbDesc, sizeof(vbDesc));
		vbDesc.Usage = D3D11_USAGE_DEFAULT;
		vbDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;		// vertex buffer를 사용하는데 쓸 것이라는 걸 알려주기.
		vbDesc.ByteWidth = static_cast<uint32>(sizeof(Vertex) * m_vertices.size());
		vbDesc.CPUAccessFlags = 0;

		// 정점 버퍼 생성
		D3D11_SUBRESOURCE_DATA vbData;
		ZeroMemory(&vbData, sizeof(vbData));
		vbData.pSysMem = m_vertices.data();		// 배열 데이터 할당.
		HR_T(m_device->CreateBuffer(&vbDesc, &vbData, m_vertexBuffer.GetAddressOf()));
	}

	// IndexBuffer
	{
		// 인덱스 버퍼 정보 설정
		D3D11_BUFFER_DESC ibDesc;
		ZeroMemory(&ibDesc, sizeof(ibDesc));
		ibDesc.Usage = D3D11_USAGE_DEFAULT;
		ibDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		ibDesc.ByteWidth = static_cast<uint32>(sizeof(WORD) * m_indices.size());
		ibDesc.CPUAccessFlags = 0;

		// 인덱스 버퍼 생성
		D3D11_SUBRESOURCE_DATA ibData;
		ZeroMemory(&ibData, sizeof(ibData));
		ibData.pSysMem = m_indices.data();		// 배열 데이터 할당.
		HR_T(m_device->CreateBuffer(&ibDesc, &ibData, m_indexBuffer.GetAddressOf()));
	}
}

// Render() 에서 파이프라인에 바인딩할 InputLayout 생성 	
void RenderCube::CreateInputLayout()
{
	D3D11_INPUT_ELEMENT_DESC layout[] =  // 인풋 레이아웃은 버텍스 쉐이더가 입력받을 데이터의 형식을 지정한다.
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0}
	};

	const int32 count = sizeof(layout) / sizeof(D3D11_INPUT_ELEMENT_DESC);
	HR_T(m_device->CreateInputLayout(layout, count, m_vsBlob->GetBufferPointer(), m_vsBlob->GetBufferSize(), m_inputLayout.GetAddressOf()));
}

void RenderCube::CreateConstantBuffer()
{
	// 상수 버퍼 정보 생성
	D3D11_BUFFER_DESC cbDesc;
	ZeroMemory(&cbDesc, sizeof(cbDesc));
	cbDesc.Usage = D3D11_USAGE_DEFAULT; // D3D11_USAGE_DYNAMIC;	// CPU_Write + GPU_Read 둘 다 가능하게 만들었다. 이걸로 바꾸기 .오류 안나면.
	cbDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;	// 상수 버퍼 용도로 활용
	cbDesc.ByteWidth = sizeof(TransformData);
	cbDesc.CPUAccessFlags = 0;		// cpu 도 접근가능하게.

	// 상수 버퍼 생성
	HR_T(m_device->CreateBuffer(&cbDesc, nullptr, m_transformBuffer.GetAddressOf()));
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

