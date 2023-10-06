#include "Fourth_Shared.fxh"

/**
 * \brief Pixel Shader
 * \param input 
 * \return 
 */

Texture2D texture0 : register(t0);
SamplerState sampler0 : register(s0);

float4 main(VS_OUTPUT input) : SV_Target
{
    float4 textureColor = texture0.Sample(sampler0, input.uv);
    float4 finalColor = saturate(dot(input.normal, (float3) -lightDir) * lightColor);
    finalColor.a = 1.f;

    finalColor *= textureColor;

    return finalColor;
}