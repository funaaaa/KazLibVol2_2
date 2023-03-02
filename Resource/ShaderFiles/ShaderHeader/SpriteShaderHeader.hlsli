//�X�v���C�g�p�̃V�F�[�_�[�w�b�_�[
cbuffer cbuff0 : register(b0)
{
    float4 color; //�F
    matrix mat; //3D�ϊ��s��
}

struct VSOutput
{
    float4 svpos : SV_POSITION; //�V�X�e���p���_���W
    float2 uv : TEXCOORD; //uv�l
};

struct ClipTexOutput
{
    float4 svpos : SV_POSITION; //�V�X�e���p���_���W
    float4 screenPos : TEXCOORD; //uv�l
};

float4 ComputeNonStereoScreenPos(float4 pos)
{
    float4 o = pos * 0.5f;
    o.xy = float2(o.x, o.y * -1.0) + o.w;
    o.zw = pos.zw;
    return o;
};