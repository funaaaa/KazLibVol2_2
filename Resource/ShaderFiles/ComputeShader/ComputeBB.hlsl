#include"../ShaderHeader/KazMathHeader.hlsli"
#include"../ShaderHeader/MeshCollision.hlsli"

//���_���W
RWStructuredBuffer<float4> vertciesData : register(u0);
//�o��
RWStructuredBuffer<BoundingBoxData> bbPosData : register(u1);

cbuffer RootConstants : register(b0)
{
    matrix scaleRotaMat;
    uint vertNum;
};

//���f���̌`�ɉ�����BoundingBox�����ׂɂ�XYZ���ꂼ��̎��̍ő�l�A�ŏ��l���v�Z����
[numthreads(1024, 1, 1)]
void CSmain(uint3 groupId : SV_GroupID, uint groupIndex : SV_GroupIndex,uint3 groupThreadID : SV_GroupThreadID)
{    
    uint index = ThreadGroupIndex(groupId,groupIndex,groupThreadID,1024);
    if(vertNum <= index)
    {
        return;
    }
    
    float3 worldPos = GetPos(vertciesData[index].xyz,float3(0,0,0)).xyz;

    float3 vertPos = float3(80.0f,50.0f,220.0f);

    //�ŏ��l�v�Z
    bbPosData[0].minPos = MinFloat3(-vertPos, worldPos);
    //�ő�l�v�Z
    bbPosData[0].maxPos = MaxFloat3(vertPos, worldPos);
}
