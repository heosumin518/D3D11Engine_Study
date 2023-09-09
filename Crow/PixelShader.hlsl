#include "Shared.fxh"

float4 PS(VS_OUTPUT input) : SV_Target
{
	// 여기서 픽셀마다 무언가 조명 처리를 할 예정..

	return input.color;
}