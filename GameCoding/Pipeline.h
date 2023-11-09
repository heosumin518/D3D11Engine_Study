#pragma once
///
/// 렌더링 파이프라인을 묘사하는 클래스
///	DeviceContext 가 핵심 요소.
///


// 파이프라인 내부에서 '공용'으로 사용하는 요소들
// 아래 요소들을 모두 고쳐주어야 하는 것이 아니라 이러한 요소들을 수정하여 파이프라인을 입맞대로 수정할 수 있다.
// Pipeline 클래스를 '대포' 로 친다면 PipelineInfo는 '포탄' 이라고 할 수 있다.
// '대포'의 '포탄'을 입맛대로 갈아끼워주는 것.
struct PipelineInfo
{
	shared_ptr<InputLayout> inputLayout;
	shared_ptr<VertexShader> vertexShader;
	shared_ptr<PixelShader> pixelShader;
	shared_ptr<RasterizerState> rasterizerState;
	shared_ptr<BlendState> blendState;
	D3D11_PRIMITIVE_TOPOLOGY topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	// constantBuffer 를 채워주는 부분이 Info 에 없는 이유는 공통적으로 사용될 요소인지 명확하지 않기 때문에다.
	// 그렇기 때문에 필요하면 물체마다 세팅하도록 한다. (SharedResource 도 마찬가지.)
};

class Pipeline
{
public:
	Pipeline(ComPtr<ID3D11DeviceContext> deviceContext);
	~Pipeline();

	void UpdatePipeline(PipelineInfo info);

	// 공용적이지 않은 부분들은 옵션을 주어서 그때그때 갈아끼울 수 있도록 함수로 만들어둔다.
	void SetVertexBuffer(shared_ptr<VertexBuffer> buffer);
	void SetIndexBuffer(shared_ptr<IndexBuffer> buffer);

	template <typename T>
	void SetConstantBuffer(uint32 slot, uint32 scope, shared_ptr<ConstantBuffer<T>> buffer)
	{
		// constantBuffer 는 VS 단계에서만 쓰이는 것이 아니라 PS 단계에서도 쓰일 수 있기 때문에 scope 를 설정. bigflag 방식을 사용한다.
		if (scope & SS_VertexShader)
			_deviceContext->VSSetConstantBuffers(slot, 1, buffer->GetComPtr().GetAddressOf());

		if (scope & SS_PixelShader)
			_deviceContext->PSSetConstantBuffers(slot, 1, buffer->GetComPtr().GetAddressOf());
	}

	void SetTexture(uint32 slot, uint32 scope, shared_ptr<Texture> texture);
	void SetSamplerState(uint32 slot, uint32 scope, shared_ptr<SamplerState> samplerState);

	void Draw(uint32 vertexCount, uint32 startVertexLocation);
	void DrawIndexed(uint32 indexCount, uint32 startIndexLocation, uint32 baseVertexLocation);
private:
	ComPtr<ID3D11DeviceContext> _deviceContext;
};

inline void Pipeline::Draw(uint32 vertexCount, uint32 startVertexLocation)
{
	_deviceContext->Draw(vertexCount, startVertexLocation);
}

inline void Pipeline::DrawIndexed(uint32 indexCount, uint32 startIndexLocation, uint32 baseVertexLocation)
{
	_deviceContext->DrawIndexed(indexCount, startIndexLocation, baseVertexLocation);
}
