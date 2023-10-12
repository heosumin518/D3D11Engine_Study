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
    float4 textureDiffuse = texture0.Sample(sampler0, input.m_uv);

    float3 diffuse = saturate(input.m_diffuse) * textureDiffuse * m_lightDiffuse;

    float3 reflection = normalize(input.m_reflection);
    float3 viewDir = normalize(input.m_viewDir);
    // VS 에서 이미 단위벡터로 만들었던 벡터들을 다시 정규화 해주는 이유는
    // 보간기를 거치는 동안 그 값이 흐트러질 수 있기 때문이다.

    float3 specular = 0;

    if (diffuse.x > 0)  // 난반사광 양이 0% 이상일 때에만 정반사광을 계산한다.
    {                   // 난반사광이 존재하지 않는 표면에는 이미 빛이 닿지 않으므로 정반사광이 존재할 수 없다.
        if (m_useBlinnPhong == true)
        {
    		specular = saturate(dot(reflection, -input.m_worldNormal));
        }
        else
        {
    		specular = saturate(dot(reflection, -viewDir));
        }
        specular = pow(specular, m_lightSpecularPower) * m_lightSpecular;
    }

    //float3 ambient = float3(0.1f, 0.1f, 0.1f);
    float3 ambient = m_lightAmbient;

    // 최종 색상 = 주변광 + 난반사광 + 정반사광
    float4 finalColor = float4(ambient + diffuse + specular, 1);

    return finalColor;
}