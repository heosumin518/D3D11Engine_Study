#include "Fourth_Shared.fxh"

/**
 * \brief Vertex Shader
 * \param input 
 * \return 
 */
VS_OUTPUT main(VS_INPUT input)
{
    VS_OUTPUT output = (VS_OUTPUT) 0;
    output.position = mul(input.position, world);
    output.position = mul(output.position, view);
    output.position = mul(output.position, projection);
    output.normal = mul(float4(input.normal, 1), world).xyz;

    return output;
}