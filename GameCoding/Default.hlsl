
struct VS_INPUT
{
	float4 position : POSITION;
	//float4 color : COLOR;
	float2 uv : TEXCOORD;
};

struct VS_OUTPUT
{
	float4 position : SV_POSITION;
	//float4 color : COLOR;
	float2 uv : TEXCOORD;
};

cbuffer CameraData : register(b0)
{
	row_major matrix matView;
	row_major matrix matProjection;
}

cbuffer TransformData : register(b1)
{
	row_major matrix matWorld;
}

cbuffer AnimationData : register(b2)
{
    float2 spriteOffset;
    float2 spriteSize;
    float2 textureSize;
    float useAnimation;
	// 여기서는 굳이 더미 데이터를 만들 필요는 없다.
	// CPU에서 GPU로 데이터가 넘어갈 때만 맞춰주면 된다.
}

// IA - VS - RS - PS - OM
VS_OUTPUT VS(VS_INPUT input)
{
	VS_OUTPUT output;

	// WVP
	float4 position = mul(input.position, matWorld); // World
	position = mul(position, matView); // View
	position = mul(position, matProjection); // Projection

	output.position = position;
	output.uv = input.uv;

	if (useAnimation == 1.f)
	{
        output.uv *= spriteSize / textureSize;
        output.uv += spriteOffset / textureSize;
    }

	return output;
}

Texture2D texture0 : register(t0);
SamplerState sampler0 : register(s0);

float4 PS(VS_OUTPUT input) : SV_Target
{
	float4 color = texture0.Sample(sampler0, input.uv);
	
	return color;
}