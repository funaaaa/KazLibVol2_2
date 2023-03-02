#include"../ShaderHeader/KazMathHeader.hlsli"
#include"../ShaderHeader/MeshCollision.hlsli"
#include"../ShaderHeader/GPUParticle.hlsli"


//�p�[�e�B�N�������蔻��
RWStructuredBuffer<ParticleHitBoxData> hitBoxData : register(u0);
//���������C���f�b�N�X
RWStructuredBuffer<MeshSphereHitData> hitIndexData : register(u1);
//�o��
RWStructuredBuffer<ParticleHitData> inputGPUParticleData : register(u2);

RWStructuredBuffer<float3> larpPosData : register(u3);

cbuffer RootConstants : register(b0)
{
    uint hitMaxNum;
};

bool BoolUint3(uint3 A,uint3 B)
{
    return A.x == B.x && A.y == B.y && A.z == B.z;
}

//����̓����蔻��Ƃ��Ƃ�
bool CheckLinkHitBox(uint3 BASE_HIT_INDEX,uint3 CHECK_HIT_INDEX)
{
    bool linkHitFlagArray[7];
    linkHitFlagArray[0] = BoolUint3(CHECK_HIT_INDEX, BASE_HIT_INDEX);                    //�^��
    linkHitFlagArray[1] = BoolUint3(CHECK_HIT_INDEX, BASE_HIT_INDEX + uint3(0,1,0));  //��
    linkHitFlagArray[2] = BoolUint3(CHECK_HIT_INDEX, BASE_HIT_INDEX + uint3(0,-1,0)); //��
    linkHitFlagArray[3] = BoolUint3(CHECK_HIT_INDEX, BASE_HIT_INDEX + uint3(-1,0,0)); //��
    linkHitFlagArray[4] = BoolUint3(CHECK_HIT_INDEX, BASE_HIT_INDEX + uint3(1,0,0));  //�E
    linkHitFlagArray[5] = BoolUint3(CHECK_HIT_INDEX, BASE_HIT_INDEX + uint3(0,0,1));  //�O
    linkHitFlagArray[6] = BoolUint3(CHECK_HIT_INDEX, BASE_HIT_INDEX + uint3(0,0,-1)); //��

    for(int i = 0;i < 7; ++i)
    {
        if(linkHitFlagArray[i])
        {
            return true;
        }
    }
    return false;
}

//���b�V���p�[�e�B�N���̋������������C���f�b�N�X���Q�l�Ƀp�[�e�B�N���̋��������߂�
[numthreads(1024, 1, 1)]
void CSmain(uint3 groupId : SV_GroupID, uint groupIndex : SV_GroupIndex,uint3 groupThreadID : SV_GroupThreadID)
{
    uint index = ThreadGroupIndex(groupId,groupIndex,groupThreadID,1024);

    ParticleHitData particleData;
    particleData.pos = hitBoxData[index].pos;
    particleData.color = hitBoxData[index].color;
    particleData.id = hitBoxData[index].meshID;


    particleData.hitFlag = inputGPUParticleData[index].hitFlag;
    particleData.hitTimer = inputGPUParticleData[index].hitTimer;

    float larpVel = 0.01f;
    float3 basePos = hitBoxData[index].pos;

    //�����C���f�b�N�X�̏ꍇ�A�p�[�e�B�N���𓮂���������ǉ�����
    for(int i = 0;i < hitMaxNum; ++i)
    {
        //���b�V��ID�̊m�F
        if(particleData.id != hitIndexData[i].meshID)
        {
            continue;
        }
        //�����蔻��+�����N�t�����݂̏��������
        if(CheckLinkHitBox(hitIndexData[i].id,hitBoxData[index].id))
        {    
            float3 posHitBoxVec = hitIndexData[i].circlePos - hitIndexData[i].meshPos;
            float3 particleHitPos = hitBoxData[index].pos - hitIndexData[i].meshPos;
            float3 posParticleVec = posHitBoxVec - particleHitPos;

            //�Ȃ��p�̔��f
            float cos = dot(posParticleVec,posHitBoxVec) / (length(posParticleVec) * length(posHitBoxVec));
            float sin = acos(cos);
    
            int angle = RadianToAngle(sin) - 90;
            float rate = angle / 90.0f;

            posParticleVec.x = 0.0f;
            float3 vel = normalize(posParticleVec) * rate;
            float3 mulVel = float3(0.0f,15.5f,15.5f);
            larpVel = 0.1f;
            basePos = hitBoxData[index].pos + vel * mulVel;
            
            //�p�[�e�B�N�����̕`��,�����������ǂ������\������
            //particleData.color = float4(0.89, 0.50, 0.07,1);
            particleData.hitFlag = 1;
            particleData.hitTimer = 0;
            break;
        }
    }
    //�s��v�Z�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[
    
    larpPosData[index] = Larp(basePos,larpPosData[index],larpVel);

    //�����蔻��̌��点
    if(particleData.hitFlag)
    {
        particleData.color = float4(0.0, 0.8, 0.8,1.0);
        particleData.hitTimer += 1;
        if(1 <= particleData.hitTimer)
        {
            particleData.hitFlag = 0;
            particleData.hitTimer = 0;
        }
    }
    
    particleData.pos = larpPosData[index];
    inputGPUParticleData[index] = particleData;
}
