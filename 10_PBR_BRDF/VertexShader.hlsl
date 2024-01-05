#include "Shared.hlsli"

//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------
PS_INPUT vs_main(VS_INPUT input)
{
    PS_INPUT output = (PS_INPUT) 0;
    float4 pos = input.PositionModel;

    // 최대 4개까지의 본을 참조함. x y z w
	float4x4 matWorld;
    matWorld = mul(input.BlendWeights.x, MatrixPaletteArray[input.BlendIndices.x]);
    matWorld += mul(input.BlendWeights.y, MatrixPaletteArray[input.BlendIndices.y]);
    matWorld += mul(input.BlendWeights.z, MatrixPaletteArray[input.BlendIndices.z]);
    matWorld += mul(input.BlendWeights.w, MatrixPaletteArray[input.BlendIndices.w]);
    // 본을 사용하지 않는다면 matWorld = World;

    pos = mul(pos, World);
    //pos = mul(pos, matWorld);
	output.PositionWorld = pos.xyz;
    
    pos = mul(pos, View);
    pos = mul(pos, Projection);
    output.PositionProj = pos;
    output.TexCoord = input.TexCoord;
    output.NormalWorld = normalize(mul(input.NormalModel, (float3x3) World)); // scale 있을수 있으므로 normalize필요
    output.TangentWorld = normalize(mul(input.TangentModel, (float3x3) World)); // scale 있을수 있으므로 normalize필요
    //output.NormalWorld = normalize(mul(input.NormalModel, (float3x3) matWorld)); 
    //output.TangentWorld = normalize(mul(input.TangentModel, (float3x3) matWorld));

    return output;
}