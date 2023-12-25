#include "Shared.hlsli"

PS_INPUT vs_main(VS_INPUT input)
{
    PS_INPUT output = (PS_INPUT) 0;

    output.Pos = mul(input.Pos, World);
    output.WorldPos = output.Pos.xyz;
    output.Pos = mul(output.Pos, View);
    output.Pos = mul(output.Pos, Projection);

    output.Norm = mul(float4(input.Norm, 1), World).xyz;

    output.TexCoord = input.Tex;

    output.Norm = normalize(mul(input.Norm, (float3x3) World));

    output.TangentWorld = normalize(mul(input.TangentModel, (float3x3) World));

    return output;
}
