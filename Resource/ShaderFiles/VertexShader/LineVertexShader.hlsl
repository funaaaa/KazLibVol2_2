#include"../ShaderHeader/BasicShaderHeader.hlsli"

VSOutputLine VSmain(float4 pos : POSITION, float2 tickness : TEXCOORD)
{
    VSOutputLine op;
    op.svpos = mul(world, pos); // ���f���̒��_�����[���h���W�n�ɕϊ�
    op.svpos = mul(view, op.svpos); // ���[���h���W�n����J�������W�n�ɕϊ�
    op.svpos = mul(viewproj, op.svpos); // �J�������W�n����X�N���[�����W�n�ɕϊ�

    op.tickness = tickness;
    return op;
}