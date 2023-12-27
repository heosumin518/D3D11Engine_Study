//--------------------------------------------------------------------------------------
// Constant Buffer Variables
//--------------------------------------------------------------------------------------
Texture2D txDiffuse : register(t0);
Texture2D txNormal : register(t1);
Texture2D txSpecular : register(t2);
Texture2D txEmissive : register(t3);
Texture2D txOpacity : register(t4);

SamplerState samLinear : register(s0);

cbuffer CBuffer : register(b0)
{
    float SpecularPower;
    float Ambient;
    float cbpad[2];
}

cbuffer BoolBuffer : register(b1)
{
    int UseNormalMap;
    int UseSpecularMap;
    int UseGammaCorrection;
    int UseDiffuseMap;
    int UseEmissiveMap;
    int UseOpacityMap;
    int boolpad[2];
}

cbuffer TransformBuffer : register(b2)
{
    matrix World;
    matrix View;
    matrix Projection;
}

cbuffer LightDirBuffer : register(b3)
{
    float4 LightDir;
    float4 LightColor;
    float4 WorldCameraPos;
    float4 lightpad[1];
}

struct VS_INPUT
{
    float3 Pos : POSITION;
    float2 uv : UV;
    float3 Normal : NORMAL;
    float3 Tangent : TANGENT;
};

struct PS_INPUT
{
    float4 Pos : SV_POSITION;
    float2 uv : UV;
    float3 Normal : NORMAL;
    float3 Tangent : TANGENT;

    float3 Diffuse : TEXCOORD1;
    float3 ViewDir : TEXCOORD2;
    float3 Reflection : TEXCOORD3;
};