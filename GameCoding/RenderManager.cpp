#include "pch.h"
#include "RenderManager.h"
#include "Pipeline.h"
#include "MeshRenderer.h"
#include "Camera.h"
#include "Game.h"
#include "SceneManager.h"
#include "Scene.h"
#include "Mesh.h"
#include "Animator.h"

RenderManager::RenderManager(ComPtr<ID3D11Device> device, ComPtr<ID3D11DeviceContext> deviceContext)
	: _device(device), _deviceContext(deviceContext)
{

}

void RenderManager::Init()
{
	_pipeline = make_shared<Pipeline>(_deviceContext);

	_cameraBuffer = make_shared<ConstantBuffer<CameraData>>(_device, _deviceContext);
	_cameraBuffer->Create();
	_transformBuffer = make_shared<ConstantBuffer<TransformData>>(_device, _deviceContext);
	_transformBuffer->Create();
	_animationBuffer = make_shared<ConstantBuffer<AnimationData>>(_device, _deviceContext);
	_animationBuffer->Create();

	_rasterizerState = make_shared<RasterizerState>(_device);
	_rasterizerState->Create();
	_blendState = make_shared<BlendState>(_device);
	_blendState->Create();
	_samplerState = make_shared<SamplerState>(_device);
	_samplerState->Create();
}

void RenderManager::Update(shared_ptr<Graphics> graphics)
{
	graphics->RenderBegin();

	PushCameraData();		// 카메라 데이터 갱신
	GatherRenderableObjects();		// 씬에 있는 물체들 중 렌더링이 필요한 물체들을 가져오고
	RenderObjects();						// 그리고 렌더한다.

	graphics->RenderEnd();
}

void RenderManager::PushCameraData()
{
	_cameraData.matView = Camera::S_MatView;
	_cameraData.matProjection = Camera::S_MatProjection;
	_cameraBuffer->CopyData(_cameraData);
}

void RenderManager::PushTransformData()
{
	_transformBuffer->CopyData(_transformData);
}

void RenderManager::PushAnimationData()
{
	_animationBuffer->CopyData(_animationData);
}

void RenderManager::GatherRenderableObjects()
{
	_renderObjects.clear();

	auto& gameObjects = SCENE->GetActiveScene()->GetGameObjects();
	for (const shared_ptr<GameObject>& gameObject : gameObjects)
	{
		shared_ptr<MeshRenderer> meshRenderer = gameObject->GetMeshRenderer();
		if (meshRenderer)		// MeshRenderer 컴포넌트를 가지고 있는 오브젝트만 화면에 그린다.
			_renderObjects.push_back(gameObject);
	}
}

void RenderManager::RenderObjects()
{
	for (const shared_ptr<GameObject>& gameObject : _renderObjects)
	{
		shared_ptr<MeshRenderer> meshRenderer = gameObject->GetMeshRenderer();
		if (meshRenderer == nullptr)
			continue;

		shared_ptr<Transform> transform = gameObject->GetTransform();
		if (transform == nullptr)
			continue;

		// SRT
		_transformData.matWorld = transform->GetWorldMatrix();
		PushTransformData();

		// Animation
		shared_ptr<Animator> animator = gameObject->GetAnimator();
		if (animator)	// 오브젝트가 애니메이터 컴포넌트를 가지고 있다면
		{
			const Keyframe& keyframe = animator->GetCurrentKeyframe();		// 현재 실행되고 있는 애니메이션의 키프레임 얻어오기
			_animationData.spriteOffset = keyframe.offset;
			_animationData.spriteSize = keyframe.size;
			_animationData.textureSize = animator->GetCurrentAnimation()->GetTextureSize();
			_animationData.useAnimation = 1.f;
			PushAnimationData();

			_pipeline->SetConstantBuffer(2, SS_VertexShader, _animationBuffer);
			_pipeline->SetTexture(0, SS_PixelShader, animator->GetCurrentAnimation()->GetTexture());
		}
		else
		{		// 애니메이터를 들고있지 않다면 애니메이션 데이터를 0으로 밀어준다.
			_animationData.spriteOffset = Vec2(0.f, 0.f);
			_animationData.spriteSize = Vec2(0.f, 0.f);
			_animationData.textureSize = Vec2(0.f, 0.f);
			_animationData.useAnimation = 0.f;
			PushAnimationData();

			_pipeline->SetConstantBuffer(2, SS_VertexShader, _animationBuffer);
			// 애니메이션의 텍스처를 가져오지 않고 원래 메쉬 렌더러가 가지고 있던 텍스처를 그대로 사용한다.
			_pipeline->SetTexture(0, SS_PixelShader, meshRenderer->GetTexture());
		}

		PipelineInfo info;
		info.inputLayout = meshRenderer->GetInputLayout();
		info.vertexShader = meshRenderer->GetVertexShader();
		info.pixelShader = meshRenderer->GetPixelShader();
		info.rasterizerState = _rasterizerState;
		info.blendState = _blendState;
		_pipeline->UpdatePipeline(info);

		_pipeline->SetVertexBuffer(meshRenderer->GetMesh()->GetVertexBuffer());
		_pipeline->SetIndexBuffer(meshRenderer->GetMesh()->GetIndexBuffer());

		_pipeline->SetConstantBuffer(0, SS_VertexShader, _cameraBuffer);
		_pipeline->SetConstantBuffer(1, SS_VertexShader, _transformBuffer);

		//_pipeline->SetTexture(0, SS_PixelShader, meshRenderer->GetTexture());
		_pipeline->SetSamplerState(0, SS_PixelShader, _samplerState);

		_pipeline->DrawIndexed(meshRenderer->GetMesh()->GetIndexBuffer()->GetCount(), 0, 0);
	}
}
