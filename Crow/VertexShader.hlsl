#include "Shared.fxh"

VS_OUTPUT VS(VS_INPUT input)
{
	VS_OUTPUT output;
    output.position = mul(input.position, world);
    output.position = mul(output.position, view);
    output.position = mul(output.position, projection);
	output.color = input.color;

	return output;
}