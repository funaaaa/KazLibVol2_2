#include"../ShaderHeader/BasicShaderHeader.hlsli"

ColorOutPut VSmain(float4 pos : POSITION)
{
    ColorOutPut op;
    op.svpos = mul(world, pos); // ���f���̒��_�����[���h���W�n�ɕϊ�
    op.svpos = mul(view, op.svpos); // ���[���h���W�n����J�������W�n�ɕϊ�
    op.svpos = mul(viewproj, op.svpos); // �J�������W�n����X�N���[�����W�n�ɕϊ�

    return op;
}