#define REGISTER_DRAW b0

cbuffer BaseBuffer : register(REGISTER_DRAW)
{
    float4 color; //�F
    matrix mat; //3D�ϊ��s��
    matrix viewproj;
    matrix world;
    matrix view;
}

struct VSOutput
{
    float4 svpos : SV_POSITION; //�V�X�e���p���_���W
    float4 worldPos : TEXCOORD1; //�V�X�e���p���_���W
    float3 normal : NORMAL; //�@���x�N�g��
    float2 uv : TEXCOORD; //uv�l
};