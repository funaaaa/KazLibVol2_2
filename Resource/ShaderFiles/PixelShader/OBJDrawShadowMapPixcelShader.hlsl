#include"../ShaderHeader/OBJDrawShadowMapSHaderHeader.hlsli"

Texture2D<float4> tex : register(t0);
Texture2D<float4> shadowMap : register(t1);
SamplerState smp : register(s0);

float4 PSmain(VSOutput input) : SV_TARGET
{

    // ���C�g�r���[�X�N���[����ԋ�Ԃ���UV���W��Ԃɍ��W�ϊ�
    float2 shadowMapUV = input.posInLVP.xy / input.posInLVP.w;
    shadowMapUV *= float2(0.5f, -0.5f);
    shadowMapUV += 0.5f;

    // ���C�g�r���[�X�N���[����Ԃł�Z�l���v�Z����B
    float zInLVP = input.posInLVP.z / input.posInLVP.w;

    // �擾�����V���h�E�}�b�vUV���W���g���ĉe�����T���v�����O
    float3 shadowMapColor = 1.0f;
    if (shadowMapUV.x > 0.0f && shadowMapUV.x < 1.0f
        && shadowMapUV.y > 0.0f && shadowMapUV.y < 1.0f)
    {

        // �V���h�E�}�b�v�ɏ������܂�Ă���Z�l���r������B
        float zInShadowMap = shadowMap.Sample(smp, shadowMapUV).r;

        if (zInLVP > zInShadowMap)
        {

            // �B��Ă���B
            shadowMapColor = 100.0f;

        }

    }

    float3 light = normalize(float3(1, -1, 1)); //�E���������̃��C�g
    float light_diffuse = saturate(dot(-light, input.normal)); //����...diffuse��[0,1]�͈̔͂�Clamp����

    float3 shade_color;
    shade_color = m_ambient + 0.5; //�A���r�G���g
    shade_color += m_diffuse * light_diffuse; //�f�B�t���[�Y��

    float4 texColor = tex.Sample(smp, input.uv);

    //return float4(texColor.rgb * shade_color, texColor.a * m_alpha);
    float4 returnBuff = float4(texColor.rgb, m_alpha);

    // �e�̐F�������I�ɕς��邽�߂ɁA�B��Ă���Ƃ���̐F��100�ɂ��Ă��̏ꍇ�͎w��̐F�𖳗��������B
    if (shadowMapColor.r >= 100)
    {
        returnBuff.xyz = float3(0x32 / 255.0f, 0x2f / 255.0f, 0x2e / 255.0f);
    }

    return returnBuff;
}