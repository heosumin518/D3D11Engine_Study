//#pragma enable_d3d11_debug_symbols

cbuffer TransformData : register(b0)
{
    matrix world;
    matrix view;
    matrix projection;

    float4 lightDir;
    float4 lightColor;
}

struct VS_INPUT
{
    float4 position : POSITION;
    float2 uv : TEXCOORD0;
    float3 normal : NORMAL;
};

struct VS_OUTPUT
{
    float4 position : SV_POSITION;
    float2 uv : TEXCOORD0;
    float3 normal : NORMAL;
};

