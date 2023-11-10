#ifndef _GLOBAL_FX_
#define _GLOBAL_FX_

//////////////////
/// ConstBuffer //
//////////////////

cbuffer GlobalBuffer
{
    matrix V;
    matrix P;
};

matrix World;

Texture2D Texture0;
float3 LightDir;

#endif

