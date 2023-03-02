#include"../ShaderHeader/BaseShaderHeader.hlsli"

//struct VSOutput
//{
//    float4 svpos : SV_POSITION; //�V�X�e���p���_���W
//    float3 normal : NORMAL; //�@���x�N�g��
//    float2 uv : TEXCOORD; //uv�l
//};

struct ColorOutPut
{
    float4 svpos : SV_POSITION; //�V�X�e���p���_���W
};

struct VSin
{
    float4 pos : POSITION;
    float3 normal : NORMAL;
    float2 uv : TEXCOORD;
};

struct VSOutputColor
{
    float4 svpos : SV_POSITION;
    float3 normal : NORMAL;
    float2 uv : TEXCOORD;
};

struct VSOutputLine
{
    float4 svpos : SV_POSITION;
    float2 tickness : TEXCOORD;
};