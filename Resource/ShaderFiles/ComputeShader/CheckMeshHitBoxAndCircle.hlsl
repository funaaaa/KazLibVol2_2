#include"../ShaderHeader/KazMathHeader.hlsli"
#include"../ShaderHeader/MeshCollision.hlsli"
#include"../ShaderHeader/Collision.hlsli"
#include"../ShaderHeader/GPUParticle.hlsli"

cbuffer RootConstants : register(b0)
{
    uint cpuHitBoxNum;
    float particleRadius;
    uint meshCircleNum;
};
struct CPUHitBoxData
{
    float3 pos;
    float radius;
};

//���b�V�������蔻��
RWStructuredBuffer<MeshHitBox> meshHitBoxArrayData : register(u0);
//CPU�����蔻��
RWStructuredBuffer<CPUHitBoxData> cpuHitBoxArrayData : register(u1);
//�����̋��Ɣ�����������
AppendStructuredBuffer<MeshSphereHitData> hitBoxAppendData : register(u2);

//���b�V���p�[�e�B�N���Ƌ��̔���
[numthreads(1024, 1, 1)]
void CSmain(uint3 groupId : SV_GroupID, uint groupIndex : SV_GroupIndex,uint3 groupThreadID : SV_GroupThreadID)
{
    uint index = groupThreadID.x;
    index += 1024 * groupId.x;

    if(meshCircleNum <= index)
    {
        return;
    }

    CircleData meshHitBox;
    meshHitBox.pos = meshHitBoxArrayData[index].pos;
    meshHitBox.radius = particleRadius;

    MeshSphereHitData idData;
    idData.meshID = meshHitBoxArrayData[index].meshID;
    idData.id = meshHitBoxArrayData[index].id;

    CircleData hitBox;
    for(int i = 0;i < cpuHitBoxNum; ++i)
    {
        hitBox.pos = cpuHitBoxArrayData[i].pos;    
        hitBox.radius = cpuHitBoxArrayData[i].radius;

        if(hitBox.radius <= 0.0f)
        {
            continue;
        }
        //�����蔻�肪��ꂽ�瓖�������C���f�b�N�X��ۑ����A�Փˌ�̏����Ɏg��
        if(CheckCircleAndCircle(meshHitBox,hitBox))
        {            
            idData.meshPos = meshHitBox.pos;
            idData.circlePos = hitBox.pos;
            hitBoxAppendData.Append(idData);
            return;
        }
    }
    
    idData.meshID = -1;
    idData.id = uint3(-1,-1,-1);
    idData.meshPos = float3(0,0,0);
    idData.circlePos = float3(0,0,0);
    hitBoxAppendData.Append(idData);
}
