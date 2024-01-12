#pragma once

namespace Engine
{
	template <typename T>
	class ConstantBuffer
	{
	public:
		ConstantBuffer() {}
		~ConstantBuffer() {}

		ComPtr<ID3D11Buffer> GetComPtr() { return m_constantBuffer; }

		void Create()
		{
			D3D11_BUFFER_DESC desc;
			ZeroMemory(&desc, sizeof(desc));
			desc.Usage = D3D11_USAGE_DYNAMIC;	// CPU_Write + GPU_Read
			desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
			desc.ByteWidth = sizeof(T);
			desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

			CHECK(DEVICE->CreateBuffer(&desc, nullptr, m_constantBuffer.GetAddressOf()));
		}

		void CopyData(const T& data)
		{
			D3D11_MAPPED_SUBRESOURCE subResource;
			ZeroMemory(&subResource, sizeof(subResource));

			DC->Map(m_constantBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &subResource);
			::memcpy(subResource.pData, &data, sizeof(data));
			DC->Unmap(m_constantBuffer.Get(), 0);
		}

	private:
		ComPtr<ID3D11Buffer> m_constantBuffer;
	};

}

