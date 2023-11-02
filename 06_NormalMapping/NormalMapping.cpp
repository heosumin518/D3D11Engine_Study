#include "pch.h"
#include "NormalMapping.h"

NormalMapping::NormalMapping(const int32& width, const int32& height, const std::wstring& name)
	: GameProcessor(width, height, name)
{

}

NormalMapping::~NormalMapping()
{
	GameProcessor::UnInitImGUI();
}

void NormalMapping::Initialize()
{
	GameProcessor::CreateDeviceAndSwapChain();
	GameProcessor::CreateRenderTargetView();
	GameProcessor::SetViewport();
	GameProcessor::CreateDepthStencilView();

	CreateGeometry();
	GameProcessor::CreateVertexShader();
	CreateInputLayout();
	GameProcessor::CreatePixelShader();

	GameProcessor::CreateSamplerState();
	CreateShaderResourceView();

	CreateConstantBuffer();

	GameProcessor::InitImGUI();
}

void NormalMapping::Update()
{
	GameProcessor::Update();

	auto t = m_timer.TotalTime();

	// update camera
	{
		m_eye = XMVectorSet(m_cameraPos.x, m_cameraPos.y, m_cameraPos.z, 0.f);
		m_at = XMVectorSet(m_cameraPos.x, m_cameraPos.y + 1.f, 100.f, 0.f);
		m_up = XMVectorSet(0.f, 1.f, 0.f, 0.f);

		m_view = XMMatrixLookAtLH(m_eye, m_at, m_up);		// ViewTransform 행렬 구하기. XMMatrixLookToLH() 함수로도 구할 수 있음
		m_projection = XMMatrixPerspectiveFovLH(m_cameraFOV / 180.0f * 3.14f, g_winSizeX / static_cast<FLOAT>(g_winSizeY), m_cameraNear, m_cameraFar);		// 0.01f, 100.0f 각각 near 와 far
	}

	// update cube
	{
		Matrix scale = Matrix::CreateScale(m_cubeScale);
		Matrix spinX = Matrix::CreateRotationX(m_cubeRotateInfo.x);
		Matrix spinY = Matrix::CreateRotationY(m_cubeRotateInfo.y);
		Matrix spinZ = Matrix::CreateRotationZ(m_cubeRotateInfo.z);
		m_world = spinY * spinX * spinZ * scale;

		m_CBCube.world = XMMatrixTranspose(m_world);
		m_CBCube.view = XMMatrixTranspose(m_view);
		m_CBCube.projection = XMMatrixTranspose(m_projection);
	}

	// update light
	{
		m_CBLight.eyePos = m_cameraPos;
	}
}

void NormalMapping::Render()
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
		m_deviceContext->VSSetConstantBuffers(0, 1, m_CBTransformBuffer.GetAddressOf());
		m_deviceContext->VSSetConstantBuffers(1, 1, m_CBLightBuffer.GetAddressOf());
		m_deviceContext->VSSetConstantBuffers(2, 1, m_CBMaterialBuffer.GetAddressOf());

		// RS

		// PS
		m_deviceContext->PSSetShader(m_pixelShader.Get(), nullptr, 0);
		m_deviceContext->PSSetShaderResources(0, 1, m_diffuseRV.GetAddressOf());
		m_deviceContext->PSSetShaderResources(1, 1, m_normalRV.GetAddressOf());
		m_deviceContext->PSSetShaderResources(2, 1, m_specularRV.GetAddressOf());
		m_deviceContext->PSSetSamplers(0, 1, m_samplerState.GetAddressOf());

		m_deviceContext->PSSetConstantBuffers(0, 1, m_CBTransformBuffer.GetAddressOf());
		m_deviceContext->PSSetConstantBuffers(1, 1, m_CBLightBuffer.GetAddressOf());
		m_deviceContext->PSSetConstantBuffers(2, 1, m_CBMaterialBuffer.GetAddressOf());

		// OM
		// Render cube and Light
		m_deviceContext->UpdateSubresource(m_CBTransformBuffer.Get(), 0, nullptr, &m_CBCube, 0, 0);
		m_CBLight.direction.Normalize();
		m_deviceContext->UpdateSubresource(m_CBLightBuffer.Get(), 0, nullptr, &m_CBLight, 0, 0);
		m_deviceContext->UpdateSubresource(m_CBMaterialBuffer.Get(), 0, nullptr, &m_CBMaterial, 0, 0);
		m_deviceContext->DrawIndexed(m_indices.size(), 0, 0);
	}

	RenderImGUI();

	RenderEnd();
}

void NormalMapping::RenderImGUI()
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
		ImGui::SliderFloat3("Camera (x, y, z)", reinterpret_cast<float*>(&m_cameraPos), -100.0f, 100.0f);
		ImGui::SliderFloat("FOV", &m_cameraFOV, 0.01f, 180.0f);
		ImGui::SliderFloat("Near", &m_cameraNear, 0.01f, 10.0f);
		ImGui::SliderFloat("Far", &m_cameraFar, 1.f,10500.0f);

		ImGui::End();
	}

	// cube control window
	{
		ImGui::SetNextWindowPos(ImVec2(10, 10));
		ImGui::SetNextWindowSize(ImVec2(400, 150));		// 메뉴 창 크기 설정
		ImGui::Begin("Cube Control Panel");

		ImGui::Text("Rotate Cube");
		ImGui::SliderFloat("X", &m_cubeRotateInfo.x, 0.f, 10.0f);
		ImGui::SliderFloat("Y", &m_cubeRotateInfo.y, 0.f, 10.0f);
		ImGui::SliderFloat("Z", &m_cubeRotateInfo.z, 0.f, 10.0f);
		ImGui::SliderFloat("Scale", &m_cubeScale, 0.f, 1000.0f);

		ImGui::End();
	}

	// light control window
	{
		ImGui::SetNextWindowPos(ImVec2(10, 170));
		ImGui::SetNextWindowSize(ImVec2(400, 150));		// 메뉴 창 크기 설정
		ImGui::Begin("Light Control Panel");

		ImGui::SliderFloat3("Direction", reinterpret_cast<float*>(&m_CBLight.direction), -1.f, 1.f);
		ImGui::ColorEdit4("Ambient", reinterpret_cast<float*>(&m_CBLight.ambient));
		ImGui::ColorEdit4("Diffuse", reinterpret_cast<float*>(&m_CBLight.diffuse));
		ImGui::ColorEdit4("Specular", reinterpret_cast<float*>(&m_CBLight.specular));
		
		ImGui::End();
	}

	// material control window
	{
		ImGui::SetNextWindowPos(ImVec2(1150, 10));
		ImGui::SetNextWindowSize(ImVec2(400, 180));		// 메뉴 창 크기 설정
		ImGui::Begin("Metarial Control Panel");

		ImGui::Checkbox("UseNormalMap", &m_CBMaterial.useNormalMap);
		ImGui::Checkbox("UseSpecularMap", &m_CBMaterial.useSpecularMap);
		ImGui::ColorEdit4("Ambient", reinterpret_cast<float*>(&m_CBMaterial.ambient));
		ImGui::ColorEdit4("Diffuse", reinterpret_cast<float*>(&m_CBMaterial.diffuse));
		ImGui::ColorEdit4("Specular", reinterpret_cast<float*>(&m_CBMaterial.specular));
		ImGui::SliderFloat("SpecularPower", &m_CBMaterial.specularPower, 0.f, 2000.f);

		ImGui::End();
	}

	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}

void NormalMapping::CreateGeometry()
{
	// Vertex Data 준비
	// Render()에서 파이프라인에 바인딩할 버텍스 정보 준비
	m_vertices.resize(24);

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

	m_vertices[8].tangent = Vector3(0.f, 0.f, -1.0f);
	m_vertices[9].tangent = Vector3(0.f, 0.f, -1.0f);
	m_vertices[10].tangent = Vector3(0.f, 0.f, -1.0f);
	m_vertices[11].tangent = Vector3(0.f, 0.f, -1.0f);

	m_vertices[12].position = Vector3(1.0f, -1.0f, 1.0f); 	m_vertices[12].normal = Vector3(1.0f, 0.0f, 0.0f); m_vertices[12].uv = Vector2(1.0f, 1.0f);
	m_vertices[13].position = Vector3(1.0f, -1.0f, -1.0f);	m_vertices[13].normal = Vector3(1.0f, 0.0f, 0.0f);  m_vertices[13].uv = Vector2(0.0f, 1.0f);
	m_vertices[14].position = Vector3(1.0f, 1.0f, -1.0f); 	m_vertices[14].normal = Vector3(1.0f, 0.0f, 0.0f);  m_vertices[14].uv = Vector2(0.0f, 0.0f);
	m_vertices[15].position = Vector3(1.0f, 1.0f, 1.0f);  	m_vertices[15].normal = Vector3(1.0f, 0.0f, 0.0f);  m_vertices[15].uv = Vector2(1.0f, 0.0f);

	m_vertices[12].tangent = Vector3(0.f, 0.f, 1.0f);
	m_vertices[13].tangent = Vector3(0.f, 0.f, 1.0f);
	m_vertices[14].tangent = Vector3(0.f, 0.f, 1.0f);
	m_vertices[15].tangent = Vector3(0.f, 0.f, 1.0f);

	m_vertices[16].position = Vector3(-1.0f, -1.0f, -1.0f);	m_vertices[16].normal = Vector3(0.0f, 0.0f, -1.0f); m_vertices[16].uv = Vector2(0.0f, 1.0f);
	m_vertices[17].position = Vector3(1.0f, -1.0f, -1.0f); 	m_vertices[17].normal = Vector3(0.0f, 0.0f, -1.0f); m_vertices[17].uv = Vector2(1.0f, 1.0f);
	m_vertices[18].position = Vector3(1.0f, 1.0f, -1.0f);  	m_vertices[18].normal = Vector3(0.0f, 0.0f, -1.0f); m_vertices[18].uv = Vector2(1.0f, 0.0f);
	m_vertices[19].position = Vector3(-1.0f, 1.0f, -1.0f); 	m_vertices[19].normal = Vector3(0.0f, 0.0f, -1.0f); m_vertices[19].uv = Vector2(0.0f, 0.0f);

	m_vertices[16].tangent = Vector3(1.f, 0.f, 0.f);
	m_vertices[17].tangent = Vector3(1.f, 0.f, 0.f);
	m_vertices[18].tangent = Vector3(1.f, 0.f, 0.f);
	m_vertices[19].tangent = Vector3(1.f, 0.f, 0.f);

	m_vertices[20].position = Vector3(-1.0f, -1.0f, 1.0f);	m_vertices[20].normal = Vector3(0.0f, 0.0f, 1.0f); m_vertices[20].uv = Vector2(1.0f, 1.0f);
	m_vertices[21].position = Vector3(1.0f, -1.0f, 1.0f); 	m_vertices[21].normal = Vector3(0.0f, 0.0f, 1.0f); m_vertices[21].uv = Vector2(0.0f, 1.0f);
	m_vertices[22].position = Vector3(1.0f, 1.0f, 1.0f);  	m_vertices[22].normal = Vector3(0.0f, 0.0f, 1.0f); m_vertices[22].uv = Vector2(0.0f, 0.0f);
	m_vertices[23].position = Vector3(-1.0f, 1.0f, 1.0f); 	m_vertices[23].normal = Vector3(0.0f, 0.0f, 1.0f); m_vertices[23].uv = Vector2(1.0f, 0.0f);

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

	// VertexBuffer
	{
		// 정점 버퍼 정보 설정
		D3D11_BUFFER_DESC vbDesc;
		ZeroMemory(&vbDesc, sizeof(vbDesc));
		vbDesc.Usage = D3D11_USAGE_DEFAULT;
		vbDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;		// vertex buffer를 사용하는데 쓸 것이라는 걸 알려주기.
		vbDesc.ByteWidth = static_cast<uint32>(sizeof(Vertex) * m_vertices.size());
		vbDesc.ByteWidth = static_cast<UINT>(sizeof(Vertex) * m_vertices.size());
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
		ibDesc.ByteWidth = static_cast<UINT>(sizeof(WORD) * m_indices.size());
		ibDesc.CPUAccessFlags = 0;

		// 인덱스 버퍼 생성
		D3D11_SUBRESOURCE_DATA ibData;
		ZeroMemory(&ibData, sizeof(ibData));
		ibData.pSysMem = m_indices.data();		// 배열 데이터 할당.
		HR_T(m_device->CreateBuffer(&ibDesc, &ibData, m_indexBuffer.GetAddressOf()));
	}
}

// Render() 에서 파이프라인에 바인딩할 InputLayout 생성 
void NormalMapping::CreateInputLayout()
{
	D3D11_INPUT_ELEMENT_DESC layout[] =  // 인풋 레이아웃은 버텍스 쉐이더가 입력받을 데이터의 형식을 지정한다.
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
	};
	// AlignedByteOffset 값을 D3D11_APPEND_ALIGNED_ELEMENT 로 지정하면 버퍼에 데이터가 어떻게 배열되는지를 자동으로 알아내도록 할 수 있다.

	const int32 count = sizeof(layout) / sizeof(D3D11_INPUT_ELEMENT_DESC);
	HR_T(m_device->CreateInputLayout(layout, count, m_vsBlob->GetBufferPointer(), m_vsBlob->GetBufferSize(), m_inputLayout.GetAddressOf()));
}

void NormalMapping::CreateConstantBuffer()
{
	// Transform 상수 버퍼 정보 생성
	D3D11_BUFFER_DESC CBTransformDesc;
	ZeroMemory(&CBTransformDesc, sizeof(CBTransformDesc));
	CBTransformDesc.Usage = D3D11_USAGE_DEFAULT;
	CBTransformDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;	// 상수 버퍼 용도로 활용
	CBTransformDesc.ByteWidth = sizeof(CB_Transform);
	CBTransformDesc.CPUAccessFlags = 0;		// cpu 도 접근가능하게.
	// Transform 상수 버퍼 생성
	HR_T(m_device->CreateBuffer(&CBTransformDesc, nullptr, m_CBTransformBuffer.GetAddressOf()));

	// Light 상수 버퍼 정보 생성
	D3D11_BUFFER_DESC CBLightDesc;
	ZeroMemory(&CBLightDesc, sizeof(CBLightDesc));
	CBLightDesc.Usage = D3D11_USAGE_DEFAULT;
	CBLightDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;	
	CBLightDesc.ByteWidth = sizeof(CB_DirectionLight);
	CBLightDesc.CPUAccessFlags = 0;
	// Light 상수 버퍼 생성
	HR_T(m_device->CreateBuffer(&CBLightDesc, nullptr, m_CBLightBuffer.GetAddressOf()));

	// Material 상수 버퍼 정보 생성
	D3D11_BUFFER_DESC CBMaterialDesc;
	ZeroMemory(&CBMaterialDesc, sizeof(CBMaterialDesc));
	CBMaterialDesc.Usage = D3D11_USAGE_DEFAULT;
	CBMaterialDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;	
	CBMaterialDesc.ByteWidth = sizeof(CB_Material);
	CBMaterialDesc.CPUAccessFlags = 0;
	// Material 상수 버퍼 생성
	HRESULT hr;
	HR_T(m_device->CreateBuffer(&CBMaterialDesc, nullptr, m_CBMaterialBuffer.GetAddressOf()));
}

void NormalMapping::CreateShaderResourceView()
{
	//DirectX::TexMetadata md;
	//DirectX::ScratchImage img;
	//HR_T(::LoadFromWICFile(L"steveHead.png", WIC_FLAGS_NONE, &md, img));	// 이미지 로드	// MS에서 권장하는 함수..?
	//HR_T(::CreateShaderResourceView(m_device.Get(), img.GetImages(), img.GetImageCount(), md, m_shaderReasourceView.GetAddressOf()));

	HRESULT hr = CoInitializeEx(nullptr, COINITBASE_MULTITHREADED);
	if (FAILED(hr))
	{
	}// error

	// diffuse, normal, specular 텍스처 로딩
	//HR_T(CreateTextureFromFile(m_device.Get(), L"Bricks059_1K-JPG_Color.jpg", m_diffuseRV.GetAddressOf()));
	//HR_T(CreateTextureFromFile(m_device.Get(), L"Bricks059_1K-JPG_NormalDX.jpg", m_normalRV.GetAddressOf()));
	//HR_T(CreateTextureFromFile(m_device.Get(), L"Bricks059_Specular.png", m_specularRV.GetAddressOf()));
	//hr = CreateWICTextureFromFile(m_device.Get(), L"Bricks059_1K-JPG_Color.jpg", nullptr, m_diffuseRV.GetAddressOf());
	//hr = CreateWICTextureFromFile(m_device.Get(), L"Bricks059_1K-JPG_NormalDX.jpg", nullptr, m_normalRV.GetAddressOf());
	//hr = CreateWICTextureFromFile(m_device.Get(), L"Bricks059_Specular.png", nullptr, m_specularRV.GetAddressOf());

	DirectX::TexMetadata md;
	DirectX::ScratchImage img;
	HR_T(::LoadFromWICFile(L"../Resources/Bricks059_1K-JPG_Color.jpg", static_cast<WIC_FLAGS>(NULL), &md, img));	// 이미지 로드	// MS에서 권장하는 함수..?
	HR_T(::CreateShaderResourceView(m_device.Get(), img.GetImages(), img.GetImageCount(), md, m_diffuseRV.GetAddressOf()));
	
	HR_T(::LoadFromWICFile(L"../Resources/Bricks059_1K-JPG_NormalDX.jpg", static_cast<WIC_FLAGS>(NULL), &md, img));	// 이미지 로드	// MS에서 권장하는 함수..?
	HR_T(::CreateShaderResourceView(m_device.Get(), img.GetImages(), img.GetImageCount(), md, m_normalRV.GetAddressOf()));
	
	HR_T(::LoadFromWICFile(L"../Resources/Bricks059_Specular.png", static_cast<WIC_FLAGS>(NULL), &md, img));	// 이미지 로드	// MS에서 권장하는 함수..?
	HR_T(::CreateShaderResourceView(m_device.Get(), img.GetImages(), img.GetImageCount(), md, m_specularRV.GetAddressOf()));

}
