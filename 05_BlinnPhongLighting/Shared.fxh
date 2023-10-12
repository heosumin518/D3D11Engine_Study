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
    bool m_useBlinnPhong;
    float m_lightSpecularPower;
    float3 m_lightPad0;     // dummy
}

struct VS_INPUT
{
    float4 m_position : POSITION;
    float2 m_uv : TEXCOORD0;
    float3 m_normal : NORMAL;   // NORMAL 은 법선을 가리키는 시맨틱. 법선은 3차원 공간에서 방향을 나타내는 벡터
};

struct VS_OUTPUT
{
    float4 m_position : SV_POSITION;
    float2 m_uv : TEXCOORD0;
    float3 m_diffuse : TEXCOORD1;       // 픽셀의 RGB 값으로 출력할 것이므로 float3을 사용.
    float3 m_viewDir : TEXCOORD2;
    float3 m_reflection : TEXCOORD3;    // TODO : half 또는 reflect 벡터인데 이 이름을 쓰는게 맞을까..
    float3 m_worldNormal : NORMAL;
};

