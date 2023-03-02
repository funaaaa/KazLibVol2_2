#include"../ShaderHeader/KazMathHeader.hlsli"
#include"../ShaderHeader/Collision.hlsli"
#include"../ShaderHeader/MeshCollision.hlsli"
#include"../ShaderHeader/GPUParticle.hlsli"

cbuffer RootConstants : register(b0)
{
    float particleRadius;
    float meshHitBoxRadius;
    uint MeshHitBoxMaxNum;
};

//BB�̂��ꂼ��̎��̍ő�l�A�ŏ��l������
struct BBData
{
    float3 minPos;
    float3 maxPos;
};

//�����蔻����W
RWStructuredBuffer<MeshHitBox> hitBoxData : register(u0);
//�p�[�e�B�N�����
RWStructuredBuffer<ParticleData> particleData : register(u1);
//�p�[�e�B�N�������蔻����
AppendStructuredBuffer<ParticleHitBoxData> particleHitBoxData : register(u2);

//���̓����蔻��ƃp�[�e�B�N���̓����蔻�������āA���Ɠ���ID���p�[�e�B�N���ɕt�^����
[numthreads(1024, 1, 1)]
void CSmain(uint3 groupId : SV_GroupID, uint groupIndex : SV_GroupIndex,uint3 groupThreadID : SV_GroupThreadID)
{
    uint particleIndex = ThreadGroupIndex(groupId,groupIndex,groupThreadID,1024);  


    if(2000000 < particleIndex)
    {
        return;
    }

    CircleData particleHitBox;
    particleHitBox.pos = particleData[particleIndex].pos;
    particleHitBox.radius = particleRadius;


    bool3 particle0Flag;
    particle0Flag.x = particleHitBox.pos.x == 0.0f;
    particle0Flag.y = particleHitBox.pos.y == 0.0f;
    particle0Flag.z = particleHitBox.pos.z == 0.0f;

    if(particle0Flag.x && particle0Flag.y && particle0Flag.z)
    {
        return;
    }

  

    //���̔���ƃp�[�e�B�N���̔�����m���߂�
    for(int i = 0; i < MeshHitBoxMaxNum;++i)
    {
        CircleData meshHitBox;
        meshHitBox.pos = hitBoxData[i].pos;
        meshHitBox.radius = meshHitBoxRadius;


        //�L���Ȃ狅��ID���p�[�e�B�N����ID�ɓ����
        if(CheckCircleAndCircle(meshHitBox,particleHitBox))
        {
            ParticleHitBoxData appendData;
            appendData.pos = particleData[particleIndex].pos;
            appendData.color = particleData[particleIndex].color;            
            appendData.meshID = hitBoxData[i].meshID;
            appendData.id = hitBoxData[i].id;
            particleHitBoxData.Append(appendData);            
            //particleHitBoxData[particleIndex] = appendData;
            break;
        }
    }


    //ParticleHitBoxData appendData;
    //appendData.pos = particleData[particleIndex].pos;
    //appendData.color = particleData[particleIndex].color;            
    //appendData.meshID = hitBoxData[0].meshID;
    //appendData.id = hitBoxData[0].id;
    //particleHitBoxData[particleIndex] = appendData;
}
