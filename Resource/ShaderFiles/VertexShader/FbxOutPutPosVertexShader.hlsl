#include"../ShaderHeader/FbxShaderHeader.hlsli"
#include"../ShaderHeader/FogShaderHeader.hlsli"

FogOutPut VSmain(VSInput input)
{
	//�X�L�j���O�v�Z
    SkinOutPut skinned = ComputeSkin(input);

    FogOutPut op;
    //���[���h�@�������̃X�e�[�W�ɓn��
    op.svpos = mul(world, skinned.pos);
    op.fogFactor = op.svpos.xyz;
    op.svpos = mul(view, op.svpos);
    op.svpos = mul(viewproj, op.svpos);

	return op;
}


