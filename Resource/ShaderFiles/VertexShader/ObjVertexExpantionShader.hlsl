#include"../ShaderHeader/OBJShaderHeader.hlsli"
#include"../ShaderHeader/BaseShaderHeader.hlsli"
#include"../ShaderHeader/ExpantionShaderHeader.hlsli"

VSOutput VSmain(float4 pos : POSITION, float3 normal : NORMAL, float2 uv : TEXCOORD)
{

    VSOutput op;
    op.svpos = mul(mat, pos);
    op.normal = normal;
    op.uv = uv;


    // ���_��@�������Ɋg�傷��B
    //p.svpos.xyz += normal * expantionAmount;

    return op;
}