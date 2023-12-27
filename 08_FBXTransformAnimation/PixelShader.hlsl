#include "Shared.hlsli"

float4 ps_main(PS_INPUT input) : SV_Target
{
    float3 normal = normalize(input.Normal);
    float3 tangent = normalize(input.Tangent);
    float3 biTangent = cross(normal, tangent);

    if (UseNormalMap)
    {
        float3 normalTangentSpace = txNormal.Sample(samLinear, input.uv).rgb * 2.f - 1.f;
        float3x3 worldTransform = float3x3(tangent, biTangent, normal);
        normal = mul(normalTangentSpace, worldTransform);
        normal = normalize(normal);
    }

    float4 lightDir = normalize(LightDir);
    float3 diffuse = saturate(dot(-lightDir.xyz, normal));
    float3 reflection = reflect(lightDir.xyz, normal);

    float4 texColor = txDiffuse.Sample(samLinear, input.uv);
    if (UseGammaCorrection)
        texColor.rgb = pow(texColor.rgb, 2.2f);
    if (UseDiffuseMap)
        diffuse *= texColor;

    float3 specular = float3(0.f, 0.f, 0.f);

    if (diffuse.x > 0)
    {
        specular = saturate(dot(reflection, -normalize(input.ViewDir)));
        specular = pow(specular, SpecularPower);

        if (UseSpecularMap)
            specular *= txSpecular.Sample(samLinear, input.uv);
    }

    float3 emissive = 0;
    if (UseEmissiveMap)
        emissive = txEmissive.Sample(samLinear, input.uv);

    float opacity = 1.f;
    if (UseOpacityMap)
        opacity = txOpacity.Sample(samLinear, input.uv).a;

    float3 ambient = Ambient * texColor;
    float4 finalColor = float4(ambient + diffuse + specular + emissive, opacity) * LightColor;

    if (UseGammaCorrection)
        finalColor.rgb = pow(finalColor.rgb, 1 / 2.2f);

    return finalColor;
}
