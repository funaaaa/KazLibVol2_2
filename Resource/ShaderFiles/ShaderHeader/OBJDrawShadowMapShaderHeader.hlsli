#define REGISTER_DRAW b0

cbuffer cbuff0 : register(REGISTER_DRAW)
{
    float4 color; //�F
    matrix mat; //3D�ϊ��s��
    matrix viewproj;
    matrix world;
    matrix view;
}

cbuffer cbuff1 : register(b1)
{
    float3 m_ambient : packoffset(c0);
    float3 m_diffuse : packoffset(c1);
    float3 m_specular : packoffset(c2);
    float m_alpha : packoffset(c2.w);
}

cbuffer cbuff2 : register(b2)
{
    matrix lightCameraViewProj;
}

struct VSOutput
{
    float4 svpos : SV_POSITION; //�V�X�e���p���_���W
    float3 normal : NORMAL; //�@���x�N�g��
    float2 uv : TEXCOORD; //uv�l
    float4 posInLVP : TEXCOORD1;//���C�g�r���[�X�N���[����Ԃł̃s�N�Z�����W
};