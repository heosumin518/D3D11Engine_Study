//#pragma enable_d3d11_debug_symbols

cbuffer TransformData : register(b0)
{
    matrix world;
    matrix view;
    matrix projection;
    float4 lightDir[2];
    float4 lightColor[2];
    float4 outputColor;
}

struct VS_INPUT
{
    float4 position : POSITION;
    float3 normal : NORMAL;
};

struct VS_OUTPUT
{
    float4 position : SV_POSITION;
    float3 normal : TEXCOORD0;
};

