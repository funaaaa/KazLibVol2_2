#include"../ShaderHeader/BasicShaderHeader.hlsli"
#include"../ShaderHeader/LightHeader.hlsli"

/////////////////////////////////////////////////////////
// �P�x���o�p
/////////////////////////////////////////////////////////
/*!
 * @brief �P�x���o�p�̃s�N�Z���V�F�[�_�[
 */
float4 PSmain(VSOutput input) : SV_TARGET
{
    // step-14 �P�x�𒊏o����s�N�Z���V�F�[�_�[������
    float4 tex = mainRenderTargetTexture.Sample(Sampler,input.uv);

    float t = dot(tex.xyz, float3(0.2125f, 0.7154f, 0.0721f));

    //clip(t - 1.0f);
    return float4(t, t, t, 1);
    //return float4(input.uv, 1, 1);
}


