#include"../ShaderHeader/KazMathHeader.hlsli"
#include"../ShaderHeader/GPUParticle.hlsli"
#include"../ShaderHeader/MeshCollision.hlsli"

//���b�V����
RWStructuredBuffer<MeshHitBox> inputMeshHitBoxData : register(u0);
//�e�q�֌W
RWStructuredBuffer<matrix> motherMatData : register(u1);
//���W�o��
RWStructuredBuffer<MeshHitBox> outputMeshHitBoxData : register(u2);

[numthreads(1024, 1, 1)]
void CSmain(uint3 groupId : SV_GroupID, uint groupIndex : SV_GroupIndex,uint3 groupThreadID : SV_GroupThreadID)
{
    uint index = groupThreadID.x;
    index += 1024 * groupId.x;

    if(1000 < index)
    {
        return;
    }

    //���f���̐���������
    for(int i = 0;i < 2;++i)
    {
        if(inputMeshHitBoxData[index].meshID == i)
        {
            outputMeshHitBoxData[index] = inputMeshHitBoxData[index];
            outputMeshHitBoxData[index].pos = UpdatePos(motherMatData[i],inputMeshHitBoxData[index].pos);
        }
    }
}
