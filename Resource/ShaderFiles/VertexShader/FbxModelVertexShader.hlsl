#include"../ShaderHeader/FbxShaderHeader.hlsli"

VSOutput VSmain(VSInput input)
{
	//�X�L�j���O�v�Z
    SkinOutPut skinned = ComputeSkin(input);
	//�@���Ƀ��[���h�s��ɂ��X�P�[�����O
    float3 wnormal = normalize(mul(world, float4(skinned.normal, 0))).xyz;
    
    VSOutput op;
    //���[���h�@�������̃X�e�[�W�ɓn��
    op.svpos = mul(world, skinned.pos);
    op.svpos = mul(view, op.svpos);
    op.svpos = mul(viewproj, op.svpos);
    op.worldPos = mul(world, skinned.pos);

    op.normal = wnormal.xyz;
	op.uv = input.uv;
	return op;
}
