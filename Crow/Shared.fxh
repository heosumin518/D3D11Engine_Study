//--------------------------------------------------------------------------------------
// Constant Buffer Variables
//--------------------------------------------------------------------------------------
cbuffer ConstantBuffer : register(b0)
{
    matrix World;
    matrix View;
    matrix Projection;
}

struct VS_INPUT
{
	float4 position : POSITION;
	float4 color : COLOR;
};

struct VS_OUTPUT
{
	float4 position : SV_POSITION;
	float4 color : COLOR;
};

