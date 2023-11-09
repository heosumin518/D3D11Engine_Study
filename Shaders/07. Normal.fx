matrix World;
matrix View;
matrix Projection;
Texture2D Texture0;
float3 LightDir;

struct VertexInput
{
    float4 position : POSITION;
    float2 uv : TEXCOORD;
    float3 normal : NORMAL;
};

struct VertexOutput
{
    float4 position : SV_POSITION;
    float2 uv : TEXCOORD;
    float3 normal : NORMAL;
};

VertexOutput VS(VertexInput input)
{
    VertexOutput output;
    output.position = mul(input.position, World);
    output.position = mul(output.position, View);
    output.position = mul(output.position, Projection);
    output.normal = mul(input.normal, (float3x3)World);

    output.uv = input.uv;

    return output;
}

SamplerState Sampler0;

float4 PS(VertexOutput input) : SV_TARGET
{
    float3 normal = normalize(input.normal);
    float3 light = -LightDir;

    //return float4(1, 1, 1, 1) * dot(light, normal);

    return Texture0.Sample(Sampler0, input.uv) * dot(light, normal); // uv 에 맞추어 샘플링이 된 좌표.
}

RasterizerState FillModeWireFrame
{
    FillMode = Wireframe;
};

technique11 T0
{
    pass P0
    {
        SetVertexShader(CompileShader(vs_5_0, VS()));
        SetPixelShader(CompileShader(ps_5_0, PS()));
    }
    pass P1
    {
        SetRasterizerState(FillModeWireFrame);

        SetVertexShader(CompileShader(vs_5_0, VS()));
        SetPixelShader(CompileShader(ps_5_0, PS()));
    }
};
