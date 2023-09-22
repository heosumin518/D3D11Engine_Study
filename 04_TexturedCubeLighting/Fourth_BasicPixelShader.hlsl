#include "Fourth_Shared.fxh"

/**
 * \brief Pixel Shader
 * \param input 
 * \return 
 */

float4 main(VS_OUTPUT input) : SV_Target
{
    float4 finalColor = 0;

    //do NdotL lighting for 2 lights
    for (int i = 0; i < 2; i++)
    {
        finalColor += saturate(dot((float3) lightDir[i], input.normal) * lightColor[i]);
    }
    finalColor.a = 1;
    return finalColor;
}