#include"../ShaderHeader/OBJShaderHeader.hlsli"
#include"../ShaderHeader/FogShaderHeader.hlsli"
#include"../ShaderHeader/bgHeader.hlsli"

struct VSOutput
{
    float4 svpos : SV_POSITION; //�V�X�e���p���_���W
    float3 normal : NORMAL; //�@���x�N�g��
    float2 uv : TEXCOORD; //uv�l
};

Texture2D<float4> tex : register(t0);
SamplerState smp : register(s0);

float4 PSmain(VSOutput input) : SV_TARGET
{
    float3 light = normalize(float3(1, -1, 1)); //�E���������̃��C�g
    float light_diffuse = saturate(dot(-light, input.normal)); //����...diffuse��[0,1]�͈̔͂�Clamp����
	
    float3 shade_color;
    shade_color = m_ambient + 0.5; //�A���r�G���g
    shade_color += m_diffuse * light_diffuse; //�f�B�t���[�Y��

    float4 texColor = tex.Sample(smp, input.uv);

    //obj�ɒl������Ȃ��ׁA�萔��z�u����
    float3 tmpC = float3(0.0862745121, 0.149019614, 0.145098045);
    float tmpD = 0.003;
    float4 fog = CaluFog(input.svpos, texColor, tmpC, tmpD);
    //float4 fog = CaluFog(input.svpos, texColor, fogData.xyz, fogData.w);
    
    return float4(fog.rgb, fog.a * m_alpha);
}