#include"../ShaderHeader/OBJShaderHeader.hlsli"
#include"../ShaderHeader/BaseShaderHeader.hlsli"
#include"../ShaderHeader/InstanceHeader.hlsli"

Texture2D<float4> tex : register(t0);
SamplerState smp : register(s0);

struct OutLineData
{
    float expantionAmount;
    float4 outlineColor;
};

// ���_�g���p�̒萔�o�b�t�@
cbuffer InstanceOutLineBuffer : register(b2)
{
    OutLineData outlineData[INSTANCE_DATA_MAX];
}

float4 PSmain(InstanceOutPut input) : SV_TARGET
{
    float3 light = normalize(float3(1, -1, 1)); //�E���������̃��C�g
    float light_diffuse = saturate(dot(-light, input.normal)); //����...diffuse��[0,1]�͈̔͂�Clamp����

    float3 shade_color;
    shade_color = m_ambient + 0.5; //�A���r�G���g
    shade_color += m_diffuse * light_diffuse; //�f�B�t���[�Y��

    float4 texColor = tex.Sample(smp, input.uv);

	//return float4(texColor.rgb * shade_color, texColor.a * m_alpha);

    float4 returnBuff = float4(texColor.rgb, m_alpha);

    // �e�N�X�`���̐F�͎g��Ȃ��B
    returnBuff.rgba = 1.0f;

    returnBuff *= outlineData[input.id].outlineColor;

    return outlineData[input.id].outlineColor;
}