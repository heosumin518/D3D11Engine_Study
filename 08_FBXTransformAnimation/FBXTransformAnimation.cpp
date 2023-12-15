#include "pch.h"
#include "FBXTransformAnimation.h"

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

	GameProcessor::InitImGUI();

	CreateInputLayout();
	GameProcessor::CreateVertexShader();
	GameProcessor::CreatePixelShader();
	CreateConstantBuffer();
	GameProcessor::CreateSamplerState();
	InitMatrix();

	m_model.ReadFile()
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
		//m_projection = XMMatrixPerspectiveFovLH(m_cameraFOV / 180.0f * 3.14f, g_winSizeX / static_cast<FLOAT>(g_winSizeY), m_cameraNear, m_cameraFar);		// 0.01f, 100.0f ���� near �� far
		m_projection = XMMatrixPerspectiveFovLH(XM_PIDIV4, g_winSizeX / static_cast<FLOAT>(g_winSizeY), 1.0f, 10000.0f);		// 0.01f, 100.0f ���� near �� far
	}

	// update model
	{
		Matrix scale = Matrix::CreateScale(m_modelScale);
		Matrix rotation = Matrix::CreateFromYawPitchRoll(Vector3(XMConvertToRadians(m_rotation.x), XMConvertToRadians(m_rotation.y), 0));
		m_world = scale * rotation;
		
		m_CBModel.world = XMMatrixTranspose(m_world);
		m_CBModel.view = XMMatrixTranspose(m_view);
		m_CBModel.projection = XMMatrixTranspose(m_projection);
	}

	// update light
	{
		m_CBLight.eyePos = m_cameraPos;
	}
}

void FBXTransformAnimation::Render()
{
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
		m_deviceContext->VSSetConstantBuffers(0, 1, m_CBTransformBuffer.GetAddressOf());
		m_deviceContext->VSSetConstantBuffers(1, 1, m_CBLightBuffer.GetAddressOf());
		m_deviceContext->VSSetConstantBuffers(2, 1, m_CBMaterialBuffer.GetAddressOf());

		// RS

		// PS
		m_deviceContext->PSSetShader(m_pixelShader.Get(), nullptr, 0);
		m_deviceContext->PSSetSamplers(0, 1, m_samplerState.GetAddressOf());
		m_deviceContext->PSSetConstantBuffers(0, 1, m_CBTransformBuffer.GetAddressOf());
		m_deviceContext->PSSetConstantBuffers(1, 1, m_CBLightBuffer.GetAddressOf());
		m_deviceContext->PSSetConstantBuffers(2, 1, m_CBMaterialBuffer.GetAddressOf());

		// OM
		// Render cube and Light
		m_deviceContext->UpdateSubresource(m_CBTransformBuffer.Get(), 0, nullptr, &m_CBModel, 0, 0);
		m_CBLight.direction.Normalize();
		m_deviceContext->UpdateSubresource(m_CBLightBuffer.Get(), 0, nullptr, &m_CBLight, 0, 0);
		m_deviceContext->UpdateSubresource(m_CBMaterialBuffer.Get(), 0, nullptr, &m_CBMaterial, 0, 0);
		//m_deviceContext->DrawIndexed(m_indices.size(), 0, 0);
	}

	for(const auto& model : m_models)
	{
		//model->Render(m_deviceContext);

		for (size_t i = 0; i < model->GetMeshes().size(); i++)
		{
			size_t mi = model->GetMeshes()[i]->GetMaterialIndex();

			m_deviceContext->PSSetShaderResources(0, 1, model->GetMaterials()[mi]->GetDiffuseRV().GetAddressOf());
			m_deviceContext->PSSetShaderResources(1, 1, model->GetMaterials()[mi]->GetNormalRV().GetAddressOf());
			m_deviceContext->PSSetShaderResources(2, 1, model->GetMaterials()[mi]->GetSpecularRV().GetAddressOf());
			m_deviceContext->PSSetShaderResources(3, 1, model->GetMaterials()[mi]->GetEmissiveRV().GetAddressOf());
			m_deviceContext->PSSetShaderResources(4, 1, model->GetMaterials()[mi]->GetOpacityRV().GetAddressOf());

			m_CBMaterial.useDiffuseMap = model->GetMaterials()[mi]->GetDiffuseRV() != nullptr ? true : false;
			m_CBMaterial.useNormalMap = model->GetMaterials()[mi]->GetNormalRV() != nullptr ? true : false;
			m_CBMaterial.useSpecularMap = model->GetMaterials()[mi]->GetSpecularRV() != nullptr ? true : false;
			m_CBMaterial.useEmissiveMap = model->GetMaterials()[mi]->GetEmissiveRV() != nullptr ? true : false;
			m_CBMaterial.useOpacityMap = model->GetMaterials()[mi]->GetOpacityRV() != nullptr ? true : false;

			if (m_CBMaterial.useOpacityMap)
			{
				m_deviceContext->OMSetBlendState(m_blendState.Get(), nullptr, 0xffffffff);  // ���ĺ��� ���¼��� , �ٸ��ɼ��� �⺻��
				int a = 1;
				a = 54;
			}
			else
				m_deviceContext->OMSetBlendState(nullptr, nullptr, 0xffffffff);		// ��������, �ٸ��ɼ��� �⺻��

			m_deviceContext->UpdateSubresource(m_CBMaterialBuffer.Get(), 0, nullptr, &m_CBMaterial, 0, 0);
			m_deviceContext->IASetIndexBuffer(model->GetMeshes()[i]->GetIndexBuffer().Get(), DXGI_FORMAT_R16_UINT, 0);
			m_deviceContext->IASetVertexBuffers(
				0, 1,
				model->GetMeshes()[i]->GetVertexBuffer().GetAddressOf(),
				model->GetMeshes()[i]->GetVertexBufferStride(),
				model->GetMeshes()[i]->GetVertexBufferOffset()
			);
			m_deviceContext->DrawIndexed(model->GetMeshes()[i]->GetIndexCount(), 0, 0);
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

void FBXTransformAnimation::InitMatrix()
{
	m_world = Matrix::Identity;
	m_eye = XMVectorSet(0.0f, 0.0f, -1000.0f, 0.0f);
	m_at = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	m_up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	m_view = XMMatrixLookAtLH(m_eye, m_at, m_up);
	m_projection = XMMatrixPerspectiveFovLH(XM_PIDIV4, g_winSizeX / static_cast<FLOAT>(g_winSizeY), 1.0f, 10000.0f);

	// FBX Loading

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
	D3D11_BUFFER_DESC desc;

	// Transform ��� ���� ���� ����
	ZeroMemory(&desc, sizeof(desc));
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;	// ��� ���� �뵵�� Ȱ��
	desc.ByteWidth = sizeof(CB_Transform);
	desc.CPUAccessFlags = 0;		// cpu �� ���ٰ����ϰ�.
	// Transform ��� ���� ����
	HR_T(m_device->CreateBuffer(&desc, nullptr, m_CBTransformBuffer.GetAddressOf()));

	// Light ��� ���� ���� ����
	ZeroMemory(&desc, sizeof(desc));
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	desc.ByteWidth = sizeof(CB_DirectionLight);
	desc.CPUAccessFlags = 0;
	// Light ��� ���� ����
	HR_T(m_device->CreateBuffer(&desc, nullptr, m_CBLightBuffer.GetAddressOf()));

	// Material ��� ���� ���� ����
	ZeroMemory(&desc, sizeof(desc));
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	desc.ByteWidth = sizeof(CB_Material);
	desc.CPUAccessFlags = 0;
	// Material ��� ���� ����
	HRESULT hr;
	HR_T(m_device->CreateBuffer(&desc, nullptr, m_CBMaterialBuffer.GetAddressOf()));
}
