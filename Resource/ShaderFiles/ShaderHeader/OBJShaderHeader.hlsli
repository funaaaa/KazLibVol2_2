
/*
�����闝�R
1.�\���̂��ۂ�
2.�g��������P��
��.
���C�g�ƃI�u�W�F�N�g
���C�g�͈�x�����������Œl�����̂܂܎g���邪�A�I�u�W�F�N�g�͕p�ɂɐ؂�ւ���
�e�N�X�`���̐؂�ւ������S������...
�p�[�e�B�N�����ƈ��Z�b�g���ĕ`�悷��
*/

cbuffer cbuff1 : register(b1)
{
    float3 m_ambient : packoffset(c0);
    float3 m_diffuse : packoffset(c1);
    float3 m_specular : packoffset(c2);
    float m_alpha : packoffset(c2.w);
}

//struct VSOutput
//{
//    float4 svpos : SV_POSITION; //�V�X�e���p���_���W
//    float3 normal : NORMAL; //�@���x�N�g��
//    float2 uv : TEXCOORD; //uv�l
//};


struct VSLightOutput
{
    float4 svpos : SV_POSITION; //�V�X�e���p���_���W
    float3 normal : NORMAL; //�@���x�N�g��
    float4 worldPos : POSITION; //�@���x�N�g��
    float2 uv : TEXCOORD; //uv�l
};
