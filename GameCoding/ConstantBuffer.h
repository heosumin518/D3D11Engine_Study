#pragma once

template<typename T>
class ConstantBuffer
{
public:
	ConstantBuffer(ComPtr<ID3D11Device> device, ComPtr<ID3D11DeviceContext> deviceContext)
		: _device(device)
		, _deviceContext(deviceContext)
	{
		
	}
	~ConstantBuffer() { }

	ComPtr<ID3D11Buffer> GetComPtr() { return _constantBuffer; }

	void Create()
	{
		D3D11_BUFFER_DESC desc;
		ZeroMemory(&desc, sizeof(desc));
		desc.Usage = D3D11_USAGE_DYNAMIC; // CPU_Write + GPU_Read 둘 다 가능하게 만들었다.
		desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		desc.ByteWidth = sizeof(T);
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;	// cpu 도 접근가능하게.

		HRESULT hr = _device->CreateBuffer(&desc, nullptr, _constantBuffer.GetAddressOf());
		CHECK(hr);
	}

	void CopyData(const T& data)
	{
		D3D11_MAPPED_SUBRESOURCE subResource;
		ZeroMemory(&subResource, sizeof(subResource));

		_deviceContext->Map(_constantBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &subResource);	// 뚜껑 열기
		::memcpy(subResource.pData, &data, sizeof(data));	// cpu 에 있던 데이터를 gpu 에 떠넘기기.  고속 복사!
		_deviceContext->Unmap(_constantBuffer.Get(), 0);		// 뚜껑 닫기
	}

private:
	ComPtr<ID3D11Device> _device;
	ComPtr<ID3D11DeviceContext> _deviceContext;
	ComPtr<ID3D11Buffer> _constantBuffer;
};

