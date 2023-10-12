#include "Shared.fxh"

float4 ps_main(VS_OUTPUT input) : SV_Target
{
    return input.color;
}
