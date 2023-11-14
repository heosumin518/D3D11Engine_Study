#pragma once
#include "ConstantBuffer.h"

class Shader;

struct GlobalDesc
{
	Matrix V = Matrix::Identity;
	Matrix P = Matrix::Identity;
	Matrix VP = Matrix::Identity;
};

struct TransformDesc
{
	Matrix W = Matrix::Identity;
};

class RenderManager
{
	DECLARE_SINGLE(RenderManager);

public:
	void Init(shared_ptr<Shader> shader);
	void Update();

	void PushGlobalData(const Matrix& view, const Matrix& projection);
	void PushTransformData(const TransformDesc& desc);

private:
	shared_ptr<Shader> _shader;	

	GlobalDesc _globalDesc;		// 프레임마다 한번씩만 세팅하는 정보들
	shared_ptr<ConstantBuffer<GlobalDesc>> _globalBuffer;
	ComPtr<ID3DX11EffectConstantBuffer> _globalEffectBuffer;

	TransformDesc _transformDesc;		// 프레임마다 한번씩만 세팅하는 정보들
	shared_ptr<ConstantBuffer<TransformDesc>> _transformBuffer;
	ComPtr<ID3DX11EffectConstantBuffer> _transformEffectBuffer;
};

