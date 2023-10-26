//#pragma enable_d3d11_debug_symbols

cbuffer Transform : register(b0)
{
    matrix m_world;
    matrix m_view;
    matrix m_projection;
}

cbuffer DirectionLight : register(b1)
{
    float4 m_lightDir;
    float4 m_lightAmbient;
    float4 m_lightDiffuse;
    float4 m_lightSpecular;
    float3 m_eyePos;
    float pad0;
}

cbuffer Material : register(b2)
{
    float4 m_materialAmbient;
    float4 m_materialDiffuse;
    float4 m_materialSpecular;
    float m_specularPower;
    bool m_useNormalMap;
    bool m_useSpecularMap;
    float materialPad0;
}

struct VS_INPUT
{
    float4 m_position : POSITION;
    float3 m_normal : NORMAL; // NORMAL 은 법선을 가리키는 시맨틱. 법선은 3차원 공간에서 방향을 나타내는 벡터
    float3 m_tangent : TANGENT;     
    float2 m_uv : TEXCOORD0;
};

struct VS_OUTPUT
{
    float4 m_position : SV_POSITION;
    float2 m_uv : TEXCOORD0;
    float3 m_viewDir : TEXCOORD1;
    float3 T : TEXCOORD2;   // Tangent
    //float3 B : TEXCOORD3;   // BiTangent는 CrossProduct 계산 때문에 생략가능하다.
    float3 N : TEXCOORD3;   // Normal
};

