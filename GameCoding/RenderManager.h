#pragma once
#include "RenderHelper.h"

/// 씬 매니저 처럼 물체를 들고 있다.
///	그리는 물체들을 전용적으로 들고 있다는 차이가 있다.
///
class RenderManager
{
public:
	RenderManager(ComPtr<ID3D11Device> device, ComPtr<ID3D11DeviceContext> deviceContext);

	void Init();
	void Update(shared_ptr<Graphics> graphics);

private:
	void PushCameraData();
	void PushTransformData();
	void PushAnimationData();

	void GatherRenderableObjects();
	void RenderObjects();

private:
	ComPtr<ID3D11Device> _device;
	ComPtr<ID3D11DeviceContext> _deviceContext;
	shared_ptr<Pipeline> _pipeline;

private:
	// GPU와 통신을 하기 위한 버퍼들
	// Camera
	CameraData _cameraData;
	shared_ptr<ConstantBuffer<CameraData>> _cameraBuffer;

	// SRT // 물체마다 다를 수 있다.
	TransformData _transformData;
	shared_ptr<ConstantBuffer<TransformData>> _transformBuffer;

	// Animation
	AnimationData _animationData;
	shared_ptr<ConstantBuffer<AnimationData>> _animationBuffer;

private:
	shared_ptr<RasterizerState> _rasterizerState;
	shared_ptr<SamplerState> _samplerState;
	shared_ptr<BlendState> _blendState;

private:
	vector<shared_ptr<GameObject>> _renderObjects;
};

