#include "Shared.fxh"

float4 ps_main(VS_OUTPUT input) : SV_TARGET
{
	return input.m_color;
}