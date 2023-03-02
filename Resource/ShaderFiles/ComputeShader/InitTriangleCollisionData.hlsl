#include"../ShaderHeader/KazMathHeader.hlsli"
#include"../ShaderHeader/ExcuteIndirectHeader.hlsli"

cbuffer RootConstants : register(b0)
{
    float4 pos;
};

struct OutputData
{
    float3 trianglePos[3];
    float3 normal;
};

//���f�����_���W
RWStructuredBuffer<float4> vertciesData : register(u0);
//�O�p�`�Ɩ@�����Ƃ��ďo��
RWStructuredBuffer<OutputData> hitBoxData : register(u1);

[numthreads(1024, 1, 1)]
void CSmain(uint3 groupId : SV_GroupID, uint groupIndex : SV_GroupIndex,uint3 groupThreadID : SV_GroupThreadID)
{
    uint index = groupThreadID.x;
    index += 1024 * groupId.x;

    //�C���f�b�N�X���ȓ��Ȃ珈������
    //�O�p�`���\������C���f�b�N�X�̎w��--------------------------------------------
    uint firstVertIndex = index * 3;
    uint secondVertIndex = index * 3 + 1;
    uint thirdVertIndex = index * 3 + 2;
    //�O�p�`���\������C���f�b�N�X�̎w��--------------------------------------------

    //���_���W���烏�[���h���W�ɕϊ���----------------------------------------------
    float4 firstVertWorldPos = GetPos(vertciesData[firstVertIndex].xyz,pos.xyz);
    float4 secondVertWorldPos = GetPos(vertciesData[secondVertIndex].xyz,pos.xyz);
    float4 thirdVertWorldPos = GetPos(vertciesData[thirdVertIndex].xyz,pos.xyz);
    //���_���W���烏�[���h���W�ɕϊ�------------------------------------------------

    //�O�p�`�̏��𓾂�
    hitBoxData[index].trianglePos[0] = firstVertWorldPos.xyz;
    hitBoxData[index].trianglePos[1] = secondVertWorldPos.xyz;
    hitBoxData[index].trianglePos[2] = thirdVertWorldPos.xyz;

    //�@�����𓾂�
    float3 abVec = secondVertWorldPos.xyz - firstVertWorldPos.xyz;
    float3 bcVec = thirdVertWorldPos.xyz - secondVertWorldPos.xyz;    
    float3 normal = normalize(abVec * bcVec);
    hitBoxData[index].normal = normal;
}