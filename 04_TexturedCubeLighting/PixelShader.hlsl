#include "Shared.fxh"

/**
 * \brief Pixel Shader
 * \param input 
 * \return 
 */

Texture2D texture0 : register(t0);
SamplerState sampler0 : register(s0);

float4 ps_main(VS_OUTPUT input) : SV_Target
{
    float4 textureColor = texture0.Sample(sampler0, input.m_uv);
    float4 finalColor = saturate(dot(input.m_diffuse, (float3) -m_lightDir) * m_lightColor);
    finalColor.a = 1.f;

    finalColor *= textureColor;

    return finalColor;
}