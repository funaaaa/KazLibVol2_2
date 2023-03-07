#include"../ShaderHeader/OBJShaderHeader.hlsli"
#include"../ShaderHeader/BaseShaderHeader.hlsli"

Texture2D<float4>tex:register(t0);
SamplerState smp :register(s0);

float4 PSmain(VSOutput input) : SV_TARGET
{
    return float4(1, 1, 1, 1);
	float3 light = normalize(float3(1,-1,1));			//�E���������̃��C�g
	float light_diffuse = saturate(dot(-light, input.normal));//����...diffuse��[0,1]�͈̔͂�Clamp����
	
	float3 shade_color;
	shade_color = m_ambient + 0.5;					//�A���r�G���g
	shade_color += m_diffuse * light_diffuse; //�f�B�t���[�Y��

	float4 texColor = tex.Sample(smp, input.uv);

	//return float4(texColor.rgb * shade_color, color.a);
    return float4(texColor.rgb, m_alpha);
}