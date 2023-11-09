#pragma once

class VertexBuffer
{
public:
	VertexBuffer(ComPtr<ID3D11Device> device);
	~VertexBuffer();

	ComPtr<ID3D11Buffer> GetComPtr() { return _vertexBuffer; }
	uint32 GetStride() { return _stride; }
	uint32 GetOffset() { return _offset; }
	uint32 GetCount() { return _count; }

	template<typename T>
	void Create(const vector<T>& vertices)	// T에는 Vertex 등등의 구조체가 들어갈 것이다.
	{
		_stride = sizeof(T);
		_count = static_cast<uint32>(vertices.size());

		D3D11_BUFFER_DESC desc;
		ZeroMemory(&desc, sizeof(desc));
		desc.Usage = D3D11_USAGE_IMMUTABLE;		// GPU에서 read 만 가능한 데이터로 설정한다.
		desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;		// vertex buffer를 사용하는데 쓸 것이라는 걸 알려주기.
		desc.ByteWidth = _stride * _count;

		D3D11_SUBRESOURCE_DATA data;
		ZeroMemory(&data, sizeof(data));
		data.pSysMem = vertices.data();		// cpu 메모리에 들고 있던 버텍스 관련 정보의 첫번째 주소를 얻어온다.

		HRESULT hr = _device->CreateBuffer(&desc, &data, _vertexBuffer.GetAddressOf());
		CHECK(hr);
	}

private:
	ComPtr<ID3D11Device> _device;
	ComPtr<ID3D11Buffer> _vertexBuffer;

	uint32 _stride = 0;
	uint32 _offset = 0;
	uint32 _count = 0;
};

