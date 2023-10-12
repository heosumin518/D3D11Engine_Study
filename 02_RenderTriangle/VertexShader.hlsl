#include "Shared.fxh"

VS_OUTPUT vs_main(VS_INPUT input)
{
    VS_OUTPUT output;
    output.position = input.position;
    output.color = input.color;

    return output;
}
