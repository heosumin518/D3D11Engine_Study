#include "pch.h"
#include "RasterizerState.h"

RasterizerState::RasterizerState(ComPtr<ID3D11Device> device)
	: _device(device)
{
}

RasterizerState::~RasterizerState()
{
}

void RasterizerState::Create()
{
	D3D11_RASTERIZER_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	desc.FillMode = D3D11_FILL_SOLID;	// 삼각형 속을 채울 것인지 아니면 프레임만 보이게 할 것인지 설정.
	desc.CullMode = D3D11_CULL_BACK;	// 어느 면을 그리지 않을 것인지 설정.     // D3D11_CALL_BACK : 후면을 그리지 않도록 설정.
	desc.FrontCounterClockwise = false;	// 이걸로 앞 뒤를 판단. false 는 시계방향이 앞방향으로 설정.

	HRESULT hr = _device->CreateRasterizerState(&desc, _rasterizerState.GetAddressOf());
	CHECK(hr);
}
