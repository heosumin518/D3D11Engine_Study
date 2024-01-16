#include "pch.h"
#include "ResourceManagerDemo.h"
#include "ModelLoader.h"

ResourceManagerDemo::ResourceManagerDemo(const int32& width, const int32& height, const std::wstring& name)
	: GameProcessor(width, height, name)
{

}

ResourceManagerDemo::~ResourceManagerDemo()
{
	GameProcessor::UnInitImGUI();
}

void ResourceManagerDemo::Initialize()
{
	GameProcessor::CreateDeviceAndSwapChain();
	GameProcessor::CreateRenderTargetView();
	GameProcessor::SetViewport(); 
	GameProcessor::CreateDepthStencilView();
	GameProcessor::CreateBlendState();

	GameProcessor::CreateVertexShader();
	CreateInputLayout();
	GameProcessor::CreatePixelShader();

	GameProcessor::CreateSamplerState();

	CreateConstantBuffer();

	// fbx 파일 로드하여 모델 생성
	ModelLoader loader(m_device);
	m_model = loader.LoadModelFile("../Resources/cerberus2.fbx"); // GOSEGU


 	GameProcessor::InitImGUI();
}

void ResourceManagerDemo::Update()
{
	GameProcessor::Update();

	auto deltaTime = m_timer.DeltaTime();

	// update camera
	{
		m_eye = XMVectorSet(m_cameraPos.x, m_cameraPos.y, m_cameraPos.z, 0.f);
		m_at = XMVectorSet(m_cameraPos.x, m_cameraPos.y + 1.f, 100.f, 0.f);
		m_up = XMVectorSet(0.f, 1.f, 0.f, 0.f);

		m_view = XMMatrixLookAtLH(m_eye, m_at, m_up);		// ViewTransform 행렬 구하기. XMMatrixLookToLH() 함수로도 구할 수 있음
		//m_projection = XMMatrixPerspectiveFovLH(m_cameraFOV / 180.0f * 3.14f, g_winSizeX / static_cast<FLOAT>(g_winSizeY), m_cameraNear, m_cameraFar);		// 0.01f, 100.0f 각각 near 와 far
		m_projection = XMMatrixPerspectiveFovLH(XM_PIDIV4, g_winSizeX / static_cast<FLOAT>(g_winSizeY), 1.0f, 10000.0f);		// 0.01f, 100.0f 각각 near 와 far
	}

	// update cube
	{
		Matrix scale = Matrix::CreateScale(m_modelScale);
		Matrix rotation = Matrix::CreateFromYawPitchRoll(Vector3(XMConvertToRadians(m_rotation.x), XMConvertToRadians(m_rotation.y), XMConvertToRadians(m_rotation.z)));
		m_world = scale * rotation;
		
		m_CBModel.world = XMMatrixTranspose(m_world);
		m_CBModel.view = XMMatrixTranspose(m_view);
		m_CBModel.projection = XMMatrixTranspose(m_projection);
	}

	// update light
	{
		m_CBLight.eyePos = m_cameraPos;
	}

	m_model->Update(deltaTime);
}

void ResourceManagerDemo::Render()
{
	RenderBegin();


	// IA - VS - RS - PS - OM
	{
		uint32 stride = sizeof(BoneWeightVertex);
		uint32 offset = 0;

		// IA
		//m_deviceContext->IASetVertexBuffers(0, 1, m_vertexBuffer.GetAddressOf(), &stride, &offset);
		//m_deviceContext->IASetIndexBuffer(m_indexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0);	// INDEX값의 범위
		m_deviceContext->IASetInputLayout(m_inputLayout.Get());
		m_deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		// VS
		m_deviceContext->VSSetShader(m_vertexShader.Get(), nullptr, 0);
		m_deviceContext->VSSetConstantBuffers(0, 1, m_CBTransformBuffer.GetAddressOf());
		m_deviceContext->VSSetConstantBuffers(1, 1, m_CBLightBuffer.GetAddressOf());
		m_deviceContext->VSSetConstantBuffers(2, 1, m_CBMaterialBuffer.GetAddressOf());
		m_deviceContext->VSSetConstantBuffers(3, 1, m_CBMatPaletteBuffer.GetAddressOf());

		// RS

		// PS
		m_deviceContext->PSSetShader(m_pixelShader.Get(), nullptr, 0);
		m_deviceContext->PSSetSamplers(0, 1, m_samplerState.GetAddressOf());
		m_deviceContext->PSSetConstantBuffers(0, 1, m_CBTransformBuffer.GetAddressOf());
		m_deviceContext->PSSetConstantBuffers(1, 1, m_CBLightBuffer.GetAddressOf());
		m_deviceContext->PSSetConstantBuffers(2, 1, m_CBMaterialBuffer.GetAddressOf());
		m_deviceContext->PSSetConstantBuffers(3, 1, m_CBMatPaletteBuffer.GetAddressOf());

		// OM
		// Render cube and Light
		m_deviceContext->UpdateSubresource(m_CBTransformBuffer.Get(), 0, nullptr, &m_CBModel, 0, 0);
		m_CBLight.direction.Normalize();
		m_deviceContext->UpdateSubresource(m_CBLightBuffer.Get(), 0, nullptr, &m_CBLight, 0, 0);
		m_deviceContext->UpdateSubresource(m_CBMaterialBuffer.Get(), 0, nullptr, &m_CBMaterial, 0, 0);
		//m_deviceContext->DrawIndexed(m_indices.size(), 0, 0);
	}

	// render model
	for (size_t i = 0; i < m_model->GetMeshes().size(); i++)
	{
		size_t mi = m_model->GetMeshes()[i]->GetMaterialIndex();

		m_deviceContext->PSSetShaderResources(0, 1, m_model->GetMaterials()[mi]->GetDiffuseRV().GetAddressOf());
		m_deviceContext->PSSetShaderResources(1, 1, m_model->GetMaterials()[mi]->GetNormalRV().GetAddressOf());
		m_deviceContext->PSSetShaderResources(2, 1, m_model->GetMaterials()[mi]->GetSpecularRV().GetAddressOf());
		m_deviceContext->PSSetShaderResources(3, 1, m_model->GetMaterials()[mi]->GetEmissiveRV().GetAddressOf());
		m_deviceContext->PSSetShaderResources(4, 1, m_model->GetMaterials()[mi]->GetOpacityRV().GetAddressOf());
		m_deviceContext->PSSetShaderResources(5, 1, m_model->GetMaterials()[mi]->GetMetalnessRV().GetAddressOf());
		m_deviceContext->PSSetShaderResources(6, 1, m_model->GetMaterials()[mi]->GetRoughnessRV().GetAddressOf());

		m_CBMaterial.useDiffuseMap = m_model->GetMaterials()[mi]->GetDiffuseRV() != nullptr ? true : false;
		m_CBMaterial.useNormalMap = m_model->GetMaterials()[mi]->GetNormalRV() != nullptr ? true : false;
		m_CBMaterial.useSpecularMap = m_model->GetMaterials()[mi]->GetSpecularRV() != nullptr ? true : false;
		m_CBMaterial.useEmissiveMap = m_model->GetMaterials()[mi]->GetEmissiveRV() != nullptr ? true : false;
		m_CBMaterial.useOpacityMap = m_model->GetMaterials()[mi]->GetOpacityRV() != nullptr ? true : false;
		m_CBMaterial.useMetalnessMap = m_model->GetMaterials()[mi]->GetMetalnessRV() != nullptr ? true : false;
		m_CBMaterial.useRoughnessMap = m_model->GetMaterials()[mi]->GetRoughnessRV() != nullptr ? true : false;

		if (m_CBMaterial.useOpacityMap)
			m_deviceContext->OMSetBlendState(m_blendState.Get(), nullptr, 0xffffffff);  // 알파블렌드 상태설정 , 다른옵션은 기본값
		else
			m_deviceContext->OMSetBlendState(nullptr, nullptr, 0xffffffff);		// 설정해제, 다른옵션은 기본값

		m_CBModel.world = m_model->GetMeshes()[i]->GetNodeTransform().Transpose() * XMMatrixTranspose(m_world);

		m_model->GetMeshes()[i]->UpdateMatrixPalette(m_model->GetBones(), m_CBMatPalette.Array);

		m_deviceContext->UpdateSubresource(m_CBMatPaletteBuffer.Get(), 0, nullptr, &m_CBMatPalette, 0, 0);
		m_deviceContext->UpdateSubresource(m_CBTransformBuffer.Get(), 0, nullptr, &m_CBModel, 0, 0);
		m_deviceContext->UpdateSubresource(m_CBMaterialBuffer.Get(), 0, nullptr, &m_CBMaterial, 0, 0);

		m_deviceContext->IASetIndexBuffer(m_model->GetMeshes()[i]->GetIndexBuffer().Get(), DXGI_FORMAT_R32_UINT, 0);
		m_deviceContext->IASetVertexBuffers(
			0, 1,
			m_model->GetMeshes()[i]->GetVertexBuffer().GetAddressOf(),
			m_model->GetMeshes()[i]->GetVertexBufferStride(),
			m_model->GetMeshes()[i]->GetVertexBufferOffset()
		);
		m_deviceContext->DrawIndexed(m_model->GetMeshes()[i]->GetIndexCount(), 0, 0);
	}

	RenderImGUI();

	RenderEnd();
}

void ResourceManagerDemo::RenderImGUI()
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
		ImGui::SliderFloat3("Rotation", (float*)&m_rotation, -180, 180);

		ImGui::Text("Light");
		ImGui::SliderFloat3("LightDirection", (float*)&m_CBLight.direction, -1.0f, 1.0f);

		ImGui::Text("Material");
		ImGui::SliderFloat("MaterialSpecularPower", (float*)&m_CBMaterial.specularPower, 2.0f, 4096.0f);

		ImGui::Text("Camera");
		ImGui::SliderFloat3("Position", (float*)&m_cameraPos, -2000.0f, 2000.0f);

		ImGui::End();
	}

	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}


// Render() 에서 파이프라인에 바인딩할 InputLayout 생성 
void ResourceManagerDemo::CreateInputLayout()
{
	D3D11_INPUT_ELEMENT_DESC layout[] =  // 인풋 레이아웃은 버텍스 쉐이더가 입력받을 데이터의 형식을 지정한다.
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"BLENDINDICES", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"BLENDWEIGHTS", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
	};
	// AlignedByteOffset 값을 D3D11_APPEND_ALIGNED_ELEMENT 로 지정하면 버퍼에 데이터가 어떻게 배열되는지를 자동으로 알아내도록 할 수 있다.

	const int32 count = sizeof(layout) / sizeof(D3D11_INPUT_ELEMENT_DESC);
	HR_T(m_device->CreateInputLayout(layout, count, m_vsBlob->GetBufferPointer(), m_vsBlob->GetBufferSize(), m_inputLayout.GetAddressOf()));
}

void ResourceManagerDemo::CreateConstantBuffer()
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
	HR_T(m_device->CreateBuffer(&CBMaterialDesc, nullptr, m_CBMaterialBuffer.GetAddressOf()));

	// Matrix Palette 상수 버퍼 정보 생성
	D3D11_BUFFER_DESC CBMatrixPaletteDesc;
	ZeroMemory(&CBMatrixPaletteDesc, sizeof(CBMatrixPaletteDesc));
	CBMatrixPaletteDesc.Usage = D3D11_USAGE_DEFAULT;
	CBMatrixPaletteDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	CBMatrixPaletteDesc.ByteWidth = sizeof(CB_MatrixPalette);
	CBMatrixPaletteDesc.CPUAccessFlags = 0;
	// Matrix Palette 상수 버퍼 생성
	HR_T(m_device->CreateBuffer(&CBMatrixPaletteDesc, nullptr, m_CBMatPaletteBuffer.GetAddressOf()));
}
