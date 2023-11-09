#pragma once
class BlendState
{
public:
	BlendState(ComPtr<ID3D11Device> device);
	~BlendState();

	const float* GetBlendFactor() { return &_blendFactor; }
	uint32 GetSampleMask() { return _sampleMask; }
	ComPtr<ID3D11BlendState> GetComPtr() { return _blendState; }

	// 인자를 넘겨주지 않으면 아래 기본 설정대로 생성하도록 한다.
	void Create(D3D11_RENDER_TARGET_BLEND_DESC blendDesc =	
		{
			true,
			D3D11_BLEND_SRC_ALPHA,
			D3D11_BLEND_INV_SRC_ALPHA,
			D3D11_BLEND_OP_ADD,
			D3D11_BLEND_ONE,
			D3D11_BLEND_ZERO,
			D3D11_BLEND_OP_ADD,
			D3D11_COLOR_WRITE_ENABLE_ALL
		}, float factor = 0.f);

private:
	ComPtr<ID3D11Device> _device;
	ComPtr<ID3D11BlendState> _blendState;

	// 섞는 비율을 설정해줄 변수 두개
	float _blendFactor = 0.f;
	uint32 _sampleMask = 0xFFFFFFFF;
};

