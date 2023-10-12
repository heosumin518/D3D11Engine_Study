#include "Shared.fxh"

/**
 * \brief Vertex Shader
 * \param input 
 * \return 
 */
VS_OUTPUT vs_main(VS_INPUT input)
{
    VS_OUTPUT output = (VS_OUTPUT) 0;
    output.m_position = mul(input.m_position, m_world);
    output.m_position = mul(output.m_position, m_view);
    output.m_position = mul(output.m_position, m_projection);
    output.m_diffuse = mul(float4(input.m_normal, 1), m_world).xyz;
    output.m_uv = input.m_uv;

    return output;
}