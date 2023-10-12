//#pragma enable_d3d11_debug_symbols

cbuffer TransformData : register(b0)
{
    matrix m_world;
    matrix m_view;
    matrix m_projection;
}

cbuffer LightData : register(b1)
{
    float4 m_lightDir;
    float4 m_lightColor;
}

struct VS_INPUT
{
    float4 m_position : POSITION;
    float2 m_uv : TEXCOORD0;
    float3 m_normal : NORMAL;
};

struct VS_OUTPUT
{
    float4 m_position : SV_POSITION;
    float2 m_uv : TEXCOORD0;
    float3 m_diffuse : TEXCOORD1;
};

