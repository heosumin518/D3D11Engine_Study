#include "pch.h"
#include "Struct.h"
#include "ModelLoader.h"
#include "Mesh.h"
#include "Material.h"
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

	GameProcessor::CreateVertexShader();
	GameProcessor::CreatePixelShader();
	CreateInputLayout();

	GameProcessor::CreateSamplerState();
	CreateConstantBuffer();

	GameProcessor::InitImGUI();

	shared_ptr<ModelLoader> loader = make_shared<ModelLoader>(m_device);

	m_model = loader->LoadModelFile("../Resources/GOSEGU.fbx"); //BoxHuman

}

void FBXTransformAnimation::Update()
{
	GameProcessor::Update();

	auto deltaTime = m_timer.TotalTime();

	// update camera
	{
		m_eye = XMVectorSet(m_cameraPos[0], m_cameraPos[1], m_cameraPos[2], 0.f);
		m_at = XMVectorSet(0.f, 0.f, 1.f, 0.f);
		m_up = XMVectorSet(0.f, 1.f, 0.f, 0.f);

		m_view = XMMatrixLookAtLH(m_eye, m_at, m_up);		// ViewTransform 행렬 구하기. XMMatrixLookToLH() 함수로도 구할 수 있음
		m_projection = XMMatrixPerspectiveFovLH(m_cameraFOV / 180.0f * 3.14f, g_winSizeX / static_cast<FLOAT>(g_winSizeY), m_cameraNear, m_cameraFar);		// 0.01f, 100.0f 각각 near 와 far
	}

	// update model SRT Matrix
	{
		Matrix scale = Matrix::CreateScale(m_modelScale);
		Matrix rotation = Matrix::CreateFromYawPitchRoll(Vector3(XMConvertToRadians(m_rotation.x), XMConvertToRadians(m_rotation.y), 0));
		Matrix translate = Matrix::CreateTranslation(m_modelTransform);
		m_world = scale * rotation * translate;
	}


	m_model->Update(deltaTime);
}

void FBXTransformAnimation::Render()
{
	RenderBegin();

	m_cbBool.isUseGammaCorrection = m_isUseGamma;

	m_cbTransform.world = XMMatrixTranspose(m_world);
	m_cbTransform.view = XMMatrixTranspose(m_view);
	m_cbTransform.projection = XMMatrixTranspose(m_projection);


	// update light
	{
		m_cbConstant.ambient = m_ambient;
		m_cbConstant.specularPower = m_specularPower;
		
		m_cbLight.lightColor = m_lightColor;
		m_cbLight.lightDir = m_lightDir;
		m_cbLight.lightDir.Normalize();
		m_cbLight.worldCameraPos = XMVectorSet(m_cameraPos[0], m_cameraPos[1], m_cameraPos[2], 0.f);
	}

	// IA - VS - RS - PS - OM
	{
		// IA
		m_deviceContext->IASetInputLayout(m_inputLayout.Get());
		m_deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		// VS
		m_deviceContext->VSSetShader(m_vertexShader.Get(), nullptr, 0);
		m_deviceContext->VSSetConstantBuffers(0, 1, m_pConstantBuffer.GetAddressOf());
		m_deviceContext->VSSetConstantBuffers(1, 1, m_pBoolBuffer.GetAddressOf());
		m_deviceContext->VSSetConstantBuffers(2, 1, m_pTransformBuffer.GetAddressOf());
		m_deviceContext->VSSetConstantBuffers(3, 1, m_pLightBuffer.GetAddressOf());

		// RS

		// PS
		m_deviceContext->PSSetShader(m_pixelShader.Get(), nullptr, 0);
		m_deviceContext->PSSetSamplers(0, 1, m_samplerState.GetAddressOf());
		m_deviceContext->PSSetConstantBuffers(0, 1, m_pConstantBuffer.GetAddressOf());
		m_deviceContext->PSSetConstantBuffers(1, 1, m_pBoolBuffer.GetAddressOf());
		m_deviceContext->PSSetConstantBuffers(2, 1, m_pTransformBuffer.GetAddressOf());
		m_deviceContext->PSSetConstantBuffers(3, 1, m_pLightBuffer.GetAddressOf());

		m_deviceContext->UpdateSubresource(m_pConstantBuffer.Get(), 0, nullptr, &m_cbConstant, 0, 0);
		m_deviceContext->UpdateSubresource(m_pLightBuffer.Get(), 0, nullptr, &m_cbLight, 0, 0);

		for (UINT i = 0; i < m_model->GetMeshes().size(); i++)
		{
			UINT mi = m_model->GetMeshes()[i]->GetMaterialIndex();

			m_deviceContext->PSSetShaderResources(0, 1, m_model->GetMaterialByIndex(mi)->GetDiffuseRV().GetAddressOf());
			m_deviceContext->PSSetShaderResources(1, 1, m_model->GetMaterialByIndex(mi)->GetNormalRV().GetAddressOf());
			m_deviceContext->PSSetShaderResources(2, 1, m_model->GetMaterialByIndex(mi)->GetSpecularRV().GetAddressOf());
			m_deviceContext->PSSetShaderResources(3, 1, m_model->GetMaterialByIndex(mi)->GetEmissiveRV().GetAddressOf());
			m_deviceContext->PSSetShaderResources(4, 1, m_model->GetMaterialByIndex(mi)->GetOpacityRV().GetAddressOf());

			m_cbBool.isUseDiffuseMap = m_model->GetMaterialByIndex(mi)->GetDiffuseRV() != nullptr ? true : false;
			m_cbBool.isUseNormalMap = m_model->GetMaterialByIndex(mi)->GetNormalRV() != nullptr ? true : false;
			m_cbBool.isUseSpecularMap = m_model->GetMaterialByIndex(mi)->GetSpecularRV() != nullptr ? true : false;
			m_cbBool.isUseEmissiveMap = m_model->GetMaterialByIndex(mi)->GetEmissiveRV() != nullptr ? true : false;
			m_cbBool.isUseOpacityMap = m_model->GetMaterialByIndex(mi)->GetOpacityRV() != nullptr ? true : false;

			if (true == m_cbBool.isUseOpacityMap)
				m_deviceContext->OMSetBlendState(m_blendState.Get(), nullptr, 0xffffffff);
			else
				m_deviceContext->OMSetBlendState(nullptr, nullptr, 0xffffffff);

			//m_cbTransform.world = XMMatrixTranspose(m_world) * XMMatrixTranspose(m_model->GetMeshes()[i]->GetNodeWorldMatrix());
			//m_deviceContext->UpdateSubresource(m_pTransformBuffer.Get(), 0, nullptr, &m_cbTransform, 0, 0);
			m_deviceContext->UpdateSubresource(m_pBoolBuffer.Get(), 0, nullptr, &m_cbBool, 0, 0);

			m_deviceContext->IASetIndexBuffer(m_model->GetMeshes()[i]->GetIndexBuffer().Get(), DXGI_FORMAT_R16_UINT, 0);
			m_deviceContext->IASetVertexBuffers
			(
				0, 1,
				m_model->GetMeshes()[i]->GetVertexBuffer().GetAddressOf(),
				m_model->GetMeshes()[i]->GetVertexBufferStride(),
				m_model->GetMeshes()[i]->GetVertexBufferOffset()
			);

			m_deviceContext->DrawIndexed(m_model->GetMeshes()[i]->GetIndexCount(), 0, 0);
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

		ImGui::Text("Camera");
		ImGui::SliderFloat3("Position", m_cameraPos, -1000.f, 1000.f);
		ImGui::SliderFloat("Far", &m_cameraFar, 1.f, 10000.f);
		ImGui::SliderFloat("Near", &m_cameraNear, 0.01, 10.f);
		ImGui::SliderFloat("FOV", &m_cameraFOV, -20.f, 180.f);


		ImGui::Text("Model");
		ImGui::SliderFloat3("Position", (float*)&m_modelTransform, -1.0f, 1.0f);
		ImGui::SliderFloat2("Rotation", (float*)&m_rotation, -360.f, 360.f);
		ImGui::SliderFloat("Scale", &m_modelScale, 0.f, 5.f);
		
		ImGui::Text("Light");
		ImGui::SliderFloat3("Color", (float*)&m_lightColor, 0.0f, 1.0f);
		ImGui::SliderFloat3("Direction", (float*)&m_lightDir, 0.f, 1.0f);

		ImGui::End();
	}

	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}


// Render() 에서 파이프라인에 바인딩할 InputLayout 생성 
void FBXTransformAnimation::CreateInputLayout()
{
	D3D11_INPUT_ELEMENT_DESC layout[] =  // 인풋 레이아웃은 버텍스 쉐이더가 입력받을 데이터의 형식을 지정한다.
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"UV", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
	};
	// AlignedByteOffset 값을 D3D11_APPEND_ALIGNED_ELEMENT 로 지정하면 버퍼에 데이터가 어떻게 배열되는지를 자동으로 알아내도록 할 수 있다.

	const int32 count = sizeof(layout) / sizeof(D3D11_INPUT_ELEMENT_DESC);
	HR_T(m_device->CreateInputLayout(layout, count, m_vsBlob->GetBufferPointer(), m_vsBlob->GetBufferSize(), m_inputLayout.GetAddressOf()));
}

void FBXTransformAnimation::CreateConstantBuffer()
{
	D3D11_BUFFER_DESC desc;
	
	ZeroMemory(&desc, sizeof(desc));
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	desc.CPUAccessFlags = 0;

	desc.ByteWidth = sizeof(CB_ConstantBuffer);
	HR_T(m_device->CreateBuffer(&desc, nullptr, m_pConstantBuffer.GetAddressOf()));
	
	desc.ByteWidth = sizeof(CB_BoolBuffer);
	HR_T(m_device->CreateBuffer(&desc, nullptr, m_pBoolBuffer.GetAddressOf()));
	
	desc.ByteWidth = sizeof(CB_TransformBuffer);
	HR_T(m_device->CreateBuffer(&desc, nullptr, m_pTransformBuffer.GetAddressOf()));
	
	desc.ByteWidth = sizeof(CB_LightDirBuffer);
	HR_T(m_device->CreateBuffer(&desc, nullptr, m_pLightBuffer.GetAddressOf()));
}
