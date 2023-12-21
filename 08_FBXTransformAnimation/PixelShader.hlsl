#include "Shared.hlsli"

//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------

float4 ps_main(VS_OUTPUT input) : SV_Target
{
    float4 diffuse = input.mDiffuse * LightColor;
	
    float3 normal = normalize(input.mNormal);
    float3 specular = 0;
    
    if (diffuse.x > 0)
    {
        float3 lightdir = normalize(LightDir);
        float3 halfDirection = normalize(lightdir + input.mViewDir);
        float specularDot = saturate(dot(normal, -halfDirection));
        specular = pow(specularDot, SpecularPower) * (float3) LightColor;
    }
    
    float4 finalColor = float4(specular + diffuse.rgb, 1.f);
    
    return finalColor;
}