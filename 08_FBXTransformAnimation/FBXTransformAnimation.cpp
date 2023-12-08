#include "pch.h"
#include "FBXTransformAnimation.h"
#include "ModelLoader.h"

FBXTransformAnimation::FBXTransformAnimation(const int32& width, const int32& height, const std::wstring& name)
	: GameProcessor(width, height, name)
{

}

FBXTransformAnimation::~FBXTransformAnimation()
{
	GameProcessor::UnInitImGUI();
}

void FBXTransformAnimation::Initialize()
{
	GameProcessor::CreateDeviceAndSwapChain();
	GameProcessor::CreateRenderTargetView();
	GameProcessor::SetViewport();
	GameProcessor::CreateDepthStencilView();
	GameProcessor::CreateBlendState();

	GameProcessor::CreateVertexShader();
	CreateInputLayout();
	GameProcessor::CreatePixelShader();
	GameProcessor::CreateRasterizeState();

	GameProcessor::CreateSamplerState();

	CreateConstantBuffer();

	// fbx ���� �ε��Ͽ� �� ����
	ModelLoader loader(m_device);
	m_models.push_back(loader.LoadModelFile("../Resources/BoxHuman.fbx"));
	//m_models.push_back(loader.LoadModelFile("../Resources/zeldaPosed001.fbx"));
	//m_models.push_back(loader.LoadModelFile("../Resources/Character.fbx"));

	for (const auto& model : m_models)
	{
		model->Init(m_device.Get(), m_CBModelBuffer.Get());
	}

	GameProcessor::InitImGUI();
}

void FBXTransformAnimation::Update()
{
	GameProcessor::Update();

	auto t = m_timer.TotalTime();

	// update camera
	{
		m_eye = XMVectorSet(m_cameraPos.x, m_cameraPos.y, m_cameraPos.z, 0.f);
		m_at = XMVectorSet(m_cameraPos.x, m_cameraPos.y + 1.f, 100.f, 0.f);
		m_up = XMVectorSet(0.f, 1.f, 0.f, 0.f);

		m_view = XMMatrixLookAtLH(m_eye, m_at, m_up);		// ViewTransform ��� ���ϱ�. XMMatrixLookToLH() �Լ��ε� ���� �� ����
		m_projection = XMMatrixPerspectiveFovLH(XM_PIDIV4, g_winSizeX / static_cast<FLOAT>(g_winSizeY), 1.0f, 10000.0f);		// 0.01f, 100.0f ���� near �� far

		m_CBCamera.position = m_cameraPos;
		m_CBCoordinate.View = XMMatrixTranspose(m_view);
		m_CBCoordinate.Projection = XMMatrixTranspose(m_projection);
	}

	// update light
	{
		XMFLOAT4 lightDirection = m_directionLightDir;
		Color lightColor = m_DirectionLightColor;
		Vector3 ambientColor = m_ambientColor;
		float specularPower = m_specularPower;

		m_CBLight.Direction = lightDirection;
		m_CBLight.LightColor = lightColor;
		m_CBLight.AmbientColor = ambientColor;
		m_CBLight.SpecularPower = specularPower;
	}

	// update IsUserTextureMap
	{
		m_CBUseTextureMap.UseNormalMap = m_isNormalMap;
		m_CBUseTextureMap.UseSpecularMap = m_isSpecularMap;
		m_CBUseTextureMap.UseGammaCorrection = m_isGammaCorrection;
	}
}

void FBXTransformAnimation::Render()
{
	auto t = m_timer.TotalTime();

	RenderBegin();

	// IA - VS - RS - PS - OM
	{
		uint32 stride = sizeof(Vertex);
		uint32 offset = 0;

		// IA
		//m_deviceContext->IASetVertexBuffers(0, 1, m_vertexBuffer.GetAddressOf(), &stride, &offset);
		//m_deviceContext->IASetIndexBuffer(m_indexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0);	// INDEX���� ����
		m_deviceContext->IASetInputLayout(m_inputLayout.Get());
		m_deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		// VS
		m_deviceContext->VSSetShader(m_vertexShader.Get(), nullptr, 0);
		m_deviceContext->VSSetConstantBuffers(0, 1, m_CBCoordinateBuffer.GetAddressOf());
		m_deviceContext->VSSetConstantBuffers(1, 1, m_CBLightBuffer.GetAddressOf());
		m_deviceContext->VSSetConstantBuffers(2, 1, m_CBCameraBuffer.GetAddressOf());
		m_deviceContext->VSSetConstantBuffers(3, 1, m_CBTextureBuffer.GetAddressOf());

		// RS
		m_deviceContext->RSSetState(m_rasterizerState.Get());

		// PS
		m_deviceContext->PSSetShader(m_pixelShader.Get(), nullptr, 0);
		m_deviceContext->PSSetSamplers(0, 1, m_samplerState.GetAddressOf());
		m_deviceContext->PSSetConstantBuffers(0, 1, m_CBCoordinateBuffer.GetAddressOf());
		m_deviceContext->PSSetConstantBuffers(1, 1, m_CBLightBuffer.GetAddressOf());
		m_deviceContext->PSSetConstantBuffers(2, 1, m_CBCameraBuffer.GetAddressOf());
		m_deviceContext->PSSetConstantBuffers(3, 1, m_CBTextureBuffer.GetAddressOf());

		// OM
		m_deviceContext->UpdateSubresource(m_CBCoordinateBuffer.Get(), 0, nullptr, &m_CBCoordinate, 0, 0);
		m_deviceContext->UpdateSubresource(m_CBLightBuffer.Get(), 0, nullptr, &m_CBLight, 0, 0);
		m_deviceContext->UpdateSubresource(m_CBCameraBuffer.Get(), 0, nullptr, &m_CBCamera, 0, 0);
		m_deviceContext->UpdateSubresource(m_CBTextureBuffer.Get(), 0, nullptr, &m_CBUseTextureMap, 0, 0);
		//m_deviceContext->DrawIndexed(m_indices.size(), 0, 0);

		for (const auto& model : m_models)
		{
			model.up
		}
	}


	RenderImGUI();

	RenderEnd();
}

void FBXTransformAnimation::RenderImGUI()
{
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
	// Start the Dear ImGui frame
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	{
	//// camera control window
	//{
	//	ImGui::SetNextWindowPos(ImVec2(420, 10));
	//	ImGui::SetNextWindowSize(ImVec2(450, 150));		// �޴� â ũ�� ����
	//	ImGui::Begin("Camera Control Panel");

	//	ImGui::Text("Adjust camera position");
	//	ImGui::SliderFloat3("Camera (x, y, z)", reinterpret_cast<float*>(&m_cameraPos), -1000.0f, 1000.0f);
	//	ImGui::SliderFloat("FOV", &m_cameraFOV, 0.01f, 180.0f);
	//	ImGui::SliderFloat("Near", &m_cameraNear, 0.01f, 10.0f);
	//	ImGui::SliderFloat("Far", &m_cameraFar, 1.f, 10500.0f);

	//	ImGui::End();
	//}

	//// cube control window
	//{
	//	ImGui::SetNextWindowPos(ImVec2(10, 10));
	//	ImGui::SetNextWindowSize(ImVec2(400, 150));		// �޴� â ũ�� ����
	//	ImGui::Begin("Cube Control Panel");

	//	ImGui::Text("Rotate Cube");
	//	ImGui::SliderFloat("X", &m_cubeRotateInfo.x, 0.f, 10.0f);
	//	ImGui::SliderFloat("Y", &m_cubeRotateInfo.y, 0.f, 10.0f);
	//	ImGui::SliderFloat("Z", &m_cubeRotateInfo.z, 0.f, 10.0f);
	//	ImGui::SliderFloat("Scale", &m_modelScale, 0.f, 1000.0f);

	//	ImGui::End();
	//}

	//// light control window
	//{
	//	ImGui::SetNextWindowPos(ImVec2(10, 170));
	//	ImGui::SetNextWindowSize(ImVec2(400, 150));		// �޴� â ũ�� ����
	//	ImGui::Begin("Light Control Panel");

	//	ImGui::SliderFloat3("Direction", reinterpret_cast<float*>(&m_CBLight.direction), -1.f, 1.f);
	//	ImGui::ColorEdit4("Ambient", reinterpret_cast<float*>(&m_CBLight.ambient));
	//	ImGui::ColorEdit4("Diffuse", reinterpret_cast<float*>(&m_CBLight.diffuse));
	//	ImGui::ColorEdit4("Specular", reinterpret_cast<float*>(&m_CBLight.specular));

	//	ImGui::End();
	//}

	//// material control window
	//{
	//	ImGui::SetNextWindowPos(ImVec2(1150, 10));
	//	ImGui::SetNextWindowSize(ImVec2(400, 180));		// �޴� â ũ�� ����
	//	ImGui::Begin("Metarial Control Panel");

	//	ImGui::Checkbox("UseNormalMap", &m_CBMaterial.useNormalMap);
	//	ImGui::Checkbox("UseSpecularMap", &m_CBMaterial.useSpecularMap);
	//	ImGui::ColorEdit4("Ambient", reinterpret_cast<float*>(&m_CBMaterial.ambient));
	//	ImGui::ColorEdit4("Diffuse", reinterpret_cast<float*>(&m_CBMaterial.diffuse));
	//	ImGui::ColorEdit4("Specular", reinterpret_cast<float*>(&m_CBMaterial.specular));
	//	ImGui::SliderFloat("SpecularPower", &m_CBMaterial.specularPower, 0.f, 2000.f);

	//	ImGui::End();
	//}
	}

	{
		ImGui::Begin("Properties");

		ImGui::Text("Cube");
		ImGui::SliderFloat("Scale", (float*)&m_modelScale, 1, 100);
		ImGui::SliderFloat2("Rotation", (float*)&m_rotation, -180, 180);

		ImGui::Text("Light");
		ImGui::SliderFloat3("LightDirection", (float*)&m_CBLight.direction, -1.0f, 1.0f);
		ImGui::ColorEdit4("LightAmbient", (float*)&m_CBLight.ambient);
		ImGui::ColorEdit4("LightDiffuse", (float*)&m_CBLight.diffuse);
		ImGui::ColorEdit4("LightSpecular", (float*)&m_CBLight.specular);

		ImGui::Text("Material");
		ImGui::ColorEdit4("MaterialAmbient", (float*)&m_CBMaterial.ambient);
		ImGui::ColorEdit4("MaterialDiffuse", (float*)&m_CBMaterial.diffuse);
		ImGui::ColorEdit4("MaterialSpecular", (float*)&m_CBMaterial.specular);
		ImGui::ColorEdit4("MaterialEmissive", (float*)&m_CBMaterial.emissive);
		ImGui::SliderFloat("MaterialSpecularPower", (float*)&m_CBMaterial.specularPower, 2.0f, 4096.0f);

		ImGui::Text("Camera");
		ImGui::SliderFloat3("Position", (float*)&m_cameraPos, -2000.0f, 2000.0f);

		ImGui::End();
	}

	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}


// Render() ���� ���������ο� ���ε��� InputLayout ���� 
void FBXTransformAnimation::CreateInputLayout()
{
	D3D11_INPUT_ELEMENT_DESC layout[] =  // ��ǲ ���̾ƿ��� ���ؽ� ���̴��� �Է¹��� �������� ������ �����Ѵ�.
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
	};
	// AlignedByteOffset ���� D3D11_APPEND_ALIGNED_ELEMENT �� �����ϸ� ���ۿ� �����Ͱ� ��� �迭�Ǵ����� �ڵ����� �˾Ƴ����� �� �� �ִ�.

	const int32 count = sizeof(layout) / sizeof(D3D11_INPUT_ELEMENT_DESC);
	HR_T(m_device->CreateInputLayout(layout, count, m_vsBlob->GetBufferPointer(), m_vsBlob->GetBufferSize(), m_inputLayout.GetAddressOf()));
}

void FBXTransformAnimation::CreateConstantBuffer()
{
	// Transform ��� ���� ���� ����
	D3D11_BUFFER_DESC CBTransformDesc;
	ZeroMemory(&CBTransformDesc, sizeof(CBTransformDesc));
	CBTransformDesc.Usage = D3D11_USAGE_DEFAULT;
	CBTransformDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;	// ��� ���� �뵵�� Ȱ��
	CBTransformDesc.ByteWidth = sizeof(CB_Transform);
	CBTransformDesc.CPUAccessFlags = 0;		// cpu �� ���ٰ����ϰ�.
	// Transform ��� ���� ����
	HR_T(m_device->CreateBuffer(&CBTransformDesc, nullptr, m_CBTransformBuffer.GetAddressOf()));

	// Light ��� ���� ���� ����
	D3D11_BUFFER_DESC CBLightDesc;
	ZeroMemory(&CBLightDesc, sizeof(CBLightDesc));
	CBLightDesc.Usage = D3D11_USAGE_DEFAULT;
	CBLightDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	CBLightDesc.ByteWidth = sizeof(CB_DirectionLight);
	CBLightDesc.CPUAccessFlags = 0;
	// Light ��� ���� ����
	HR_T(m_device->CreateBuffer(&CBLightDesc, nullptr, m_CBLightBuffer.GetAddressOf()));

	// Material ��� ���� ���� ����
	D3D11_BUFFER_DESC CBMaterialDesc;
	ZeroMemory(&CBMaterialDesc, sizeof(CBMaterialDesc));
	CBMaterialDesc.Usage = D3D11_USAGE_DEFAULT;
	CBMaterialDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	CBMaterialDesc.ByteWidth = sizeof(CB_Material);
	CBMaterialDesc.CPUAccessFlags = 0;
	// Material ��� ���� ����
	HRESULT hr;
	HR_T(m_device->CreateBuffer(&CBMaterialDesc, nullptr, m_CBTextureBuffer.GetAddressOf()));
}
