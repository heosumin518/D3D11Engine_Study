#include "Shared.fxh"

VS_OUTPUT vs_main(VS_INPUT input)
{
    VS_OUTPUT output;
    output.m_position = mul(input.m_position, m_world);
    output.m_position = mul(output.m_position, m_view);
    output.m_position = mul(output.m_position, m_projection);
    output.m_color = input.m_color;

    return output;
}