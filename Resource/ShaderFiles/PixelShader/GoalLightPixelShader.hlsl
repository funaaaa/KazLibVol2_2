#include"../ShaderHeader/SpriteShaderHeader.hlsli"

Texture2D<float4> tex : register(t0);
SamplerState smp : register(s0);

cbuffer buffer1 : register(b1)
{
    float4 uvValue;
};

float4 PSmain(VSOutput input) : SV_TARGET
{
    //�����Œ萔�o�b�t�@�Œl��]����uv�̒l�����炵�Ă���
    float2 tmpUv = input.uv;
    tmpUv.y += uvValue.x;    
    float4 sprite = float4(tex.Sample(smp, tmpUv));
    
    //uv���W��y���ɋ߂Â��΋߂Â��قǓ����ɂȂ��Ă���
    sprite.a = (1.0 - input.uv.y);
    if (sprite.a < color.a)
    {
    }
    else
    {
        sprite.a = color.a;
    }
    
    return sprite;
}