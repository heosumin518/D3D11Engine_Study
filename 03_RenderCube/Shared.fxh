//--------------------------------------------------------------------------------------
// Constant Buffer Variables
//--------------------------------------------------------------------------------------
cbuffer TransformData : register(b0)
{
    matrix m_world;
    matrix m_view;
    matrix m_projection;
}

struct VS_INPUT
{
    float4 m_position : POSITION;
    float4 m_color : COLOR;
};

struct VS_OUTPUT
{
    float4 m_position : SV_POSITION;
    float4 m_color : COLOR;
};
