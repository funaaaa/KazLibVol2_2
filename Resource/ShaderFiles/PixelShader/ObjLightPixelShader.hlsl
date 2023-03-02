#include"../ShaderHeader/OBJShaderHeader.hlsli"
#include"../ShaderHeader/BaseShaderHeader.hlsli"

cbuffer cbuff2 : register(b2)
{
    float3 lightV;
    float3 blightColor;
    float3 eyePos;
};

Texture2D<float4> tex : register(t0);
SamplerState smp : register(s0);

float4 PSmain(VSLightOutput input) : SV_TARGET
{
    //���C�g�̐F
    //float3 lightColor = blightColor;    
    float4 texColor = tex.Sample(smp, input.uv);
    
    float3 ambientData = m_ambient;
    float3 diffuseData = m_diffuse;
    float3 specularData = m_specular;
    

    //����x
    const float shininess = 4.0f;
    //���_���王�_�ւ̕����x�N�g��
    float3 eyeDir = normalize(eyePos - input.worldPos.xyz);
    //���C�g�Ɍ������x�N�g���Ɩ@���̓���
    float3 dotLightNormal = dot(lightV, input.normal);
    //���ˌ��x�N�g��
    float3 reflect = normalize(-lightV + 2 * dotLightNormal * input.normal);
    
    
    //�����ˌ�
    float3 ambient = ambientData;
    //�g�U���ˌ�
    float3 diffuse = dotLightNormal * diffuseData;
    
    
    //���ʔ��ˌ�    
    float3 resultDot = dot(reflect, eyeDir);
    float3 resultSaturate = saturate(resultDot);    
    float3 specular = pow(resultSaturate, shininess) * specularData;

    //����
    float4 shaderColor;
    shaderColor.rgb = (ambient + diffuse + specular);
    shaderColor.a = m_alpha;

    return float4(shaderColor * texColor);
}