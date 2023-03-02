//�y���p�̃V�F�[�_�[�w�b�_�[
cbuffer PostEffect : register(b0)
{
    float4 bkweights[2];
}


cbuffer cbuff0 : register(b0)
{
    float4 color; //�F
    matrix mat; //3D�ϊ��s��
}

Texture2D<float4> tex : register(t0);
Texture2D<float4> effectTex : register(t1);

SamplerState smp : register(s0);

struct Output
{
    float4 svpos : SV_POSITION;
    float2 uv : TEXCOORD;
};