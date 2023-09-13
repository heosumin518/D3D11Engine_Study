#include "Shared.fxh"

VS_OUTPUT VS(VS_INPUT input)
{
	VS_OUTPUT output;
	output.position = input.position;
    output.position = mul(input.position, World);
    output.position = mul(output.position, View);
    output.position = mul(output.position, Projection);
	output.color = input.color;

	return output;
}