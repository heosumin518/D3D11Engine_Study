#include "Shared.hlsli"

PS_INPUT vs_main(VS_INPUT input)
{
    PS_INPUT output = (PS_INPUT) 0;

    output.Pos = mul(input.Pos, World);

    output.ViewDir = normalize(output.Pos.xyz - WorldCameraPos.xyz);
    output.Normal = normalize(mul(input.Normal, (float3x3) World));
    output.Tangent = input.Tangent;
    output.uv = input.uv;

    output.Pos = mul(output.Pos, View);
    output.Pos = mul(output.Pos, Projection);

    return output;
}
