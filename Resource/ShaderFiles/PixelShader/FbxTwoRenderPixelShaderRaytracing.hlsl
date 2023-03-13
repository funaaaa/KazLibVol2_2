#include"../ShaderHeader/FbxShaderHeader.hlsli"
#include"../ShaderHeader/MultiPassHeader.hlsli"

Texture2D<float4> tex : register(t0);
SamplerState smp : register(s0);

// ���C�g���pGBuffer
RWTexture2D<float4> GBuffer0 : register(u0);
RWTexture2D<float4> GBuffer1 : register(u1);
RWTexture2D<float4> RenderUAV : register(u2);

cbuffer LightDirBuffer : register(b2)
{
    float3 lightDir;
}

TwoRender PSmain(VSOutput input)
{
    float3 light = normalize(lightDir);
    float diffuse = saturate(dot(-light, input.normal));
    float brightness = diffuse + 0.5f;

    float4 texColor = float4(tex.Sample(smp, input.uv));
    
    TwoRender outPut;
    outPut.target0 = texColor * brightness;
    outPut.target0.a = color.a;
    outPut.target1 = float4(0.0f, 0.0f, 0.0f, 1.0f);
    
    // ���̎����B�{���͐�p�̎��ʗp�萔�o�b�t�@��p�ӂ���B
    // ���C�g�������邩���Ȃ�����GBuffer�ɏ������ޓ��e��ς���B
    if (color.a == 1)
    {
        
        // GBuffer�ւ̏�������
        GBuffer0[input.svpos.xy].xyz = input.worldPos.xyz;
        GBuffer0[input.svpos.xy].w = 1;
    
        // �J�����̈ʒu���r���[�s�񂩂�擾����B
        float3 eyePos = float3(15,0,0);
    
        // �J�������炱�̃|���S���܂ł̃x�N�g��
        float3 eyeDir = normalize(input.worldPos.xyz - eyePos);
    
        // ���˂�����B
        GBuffer1[input.svpos.xy].xyz = reflect(eyeDir, input.normal.xyz);
        GBuffer1[input.svpos.xy].w = 1;
        
    }
    else
    {
        
        // GBuffer�ւ̏�������
        GBuffer0[input.svpos.xy] = float4(0, 0, 0, 0);
        GBuffer1[input.svpos.xy] = float4(0, 0, 0, 0);

    }
    
    
    RenderUAV[input.svpos.xy] = outPut.target0;
    
    return outPut;
}