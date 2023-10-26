#include "Shared.fxh"

/**
 * \brief Pixel Shader
 * \param input 
 * \return 
 */

Texture2D diffuse0 : register(t0);
Texture2D normal0 : register(t1);
Texture2D specular0 : register(t2);
SamplerState sampler0 : register(s0);

float4 ps_main(VS_OUTPUT input) : SV_Target
{
    float3 worldNormal = normalize(input.N);
    float3 worldTangent = normalize(input.T);
    float3 worldBiTangent = cross(worldNormal, worldTangent);

    if (m_useNormalMap)
    {
	    // 노말 맵 텍스처로부터 노말 읽어오기 (범위: 0 ~ 1)
        float3 tangentNormal = normal0.Sample(sampler0, input.m_uv).rgb;
	    // 실제 노말 벡터의 범위를 -1 ~ 1로 확장하기
        tangentNormal = normalize(tangentNormal * 2.f - 1.f);

        // 노말 벡터를 월드공간으로 변환하기 위한 공간변환 행렬 만들기
        float3x3 TBN = float3x3(worldTangent, worldBiTangent, worldNormal); // 월드 공간의 벡터를 탄젠트 공간으로 변환하는 행렬
        //TBN = transpose(TBN); // 월드 공간으로 변환해야하므로 역행렬 구하기
        //worldNormal = mul(TBN, tangentNormal); // 탄젠트 공간의 노말 벡터를 월드 공간으로 변환

        worldNormal = mul(tangentNormal, TBN);

        // -> 역행렬을 구하지 않는다면 곱하는 순서를 바꾸어 주어야 한다. = mul(tangentNormal, TBN)
    }

    // 조명 계산
    // 1. 난반사광
    float3 lightDir = normalize(m_lightDir);
    float3 diffuse = saturate(dot(worldNormal, -lightDir)); // 난반사광의 양
    float4 albedo = diffuse0.Sample(sampler0, input.m_uv); // diffuse 맵
    diffuse = m_lightDiffuse * m_materialDiffuse * albedo * diffuse; // 빛과 머터리얼의 색상과 텍스처의 색상 그리고 난반사광의 양을 곱한다.

    // 2. 정반사광
    float3 specular = 0;
    if (diffuse.x > 0)
    {
        float3 viewDir = normalize(input.m_viewDir);

        // 스페큘러 맵
        float4 specularIntensity =  specular0.Sample(sampler0, input.m_uv);

        // 블린 퐁
        float3 halfVector = normalize(lightDir + viewDir);
        specular = saturate(dot(halfVector, -worldNormal));
        specular = pow(specular, m_specularPower) * m_lightSpecular * m_materialSpecular;

        if (m_useSpecularMap)
            specular *= specularIntensity.rgb;
    }

    //float3 ambient = float3(0.1f, 0.1f, 0.1f);
    float3 ambient = m_lightAmbient * m_materialAmbient * albedo.rgb;

    // 최종 색상 = 주변광 + 난반사광 + 정반사광
    float4 finalColor = float4(ambient + diffuse + specular, 1);

    return finalColor;
}
