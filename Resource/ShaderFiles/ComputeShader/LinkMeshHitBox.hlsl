#include"../ShaderHeader/KazMathHeader.hlsli"
#include"../ShaderHeader/MeshCollision.hlsli"

cbuffer RootConstants : register(b0)
{
    float radius;
    uint xMax;
    uint xyMax;
};

//�����蔻����W
RWStructuredBuffer<MeshHitBox> hitBoxData : register(u1);

//BoundingBox���ɔz�u���ꂽ�����m�̔��������ă����N�t������
[numthreads(1, 1, 1)]
void CSmain(uint3 groupId : SV_GroupID, uint groupIndex : SV_GroupIndex,uint3 groupThreadID : SV_GroupThreadID)
{
    uint index = ThreadGroupIndex(groupId,xMax,xyMax);
}
