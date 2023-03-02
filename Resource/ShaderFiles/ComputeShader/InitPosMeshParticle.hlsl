#include"../ShaderHeader/KazMathHeader.hlsli"
#include"../ShaderHeader/GPUParticle.hlsli"

//���_���
RWStructuredBuffer<float3> vertciesData : register(u0);
//�o��
AppendStructuredBuffer<ParticleData> outputData : register(u1);
//RWStructuredBuffer<ParticleData> outputData : register(u1);


cbuffer CommonBuffer: register(b0)
{
    //x vertNum, y bias,z perTriangleNum,w faceCountNum
    uint4 meshData;
    float4 color;
    uint motherMatIndex;
}


float CalucurateTriangleArea(float3 P0,float3 P1,float3 P2)
{
    float3 p0p1Vec = P1 - P0;
    float3 p1p2Vec = P2 - P1;
    return length(cross(p0p1Vec,p1p2Vec)) / 2.0f;
}

float CalucurateUVW(float3 P0,float3 P1,float3 ATTACK_POINT,float TRIANGLE_AREA)
{
    float3 p0p1Vec = ATTACK_POINT - P0;
    float3 p1p2Vec = ATTACK_POINT - P1;
    float area = length(cross(p0p1Vec,p1p2Vec)) / 2.0f;   
    float rate = area / TRIANGLE_AREA;
    return rate;
}

[numthreads(1024, 1, 1)]
void CSmain(uint3 groupId : SV_GroupID, uint groupIndex : SV_GroupIndex,uint3 groupThreadID : SV_GroupThreadID)
{
    uint index = groupThreadID.x;
    index += 1024 * groupId.x;

    if(meshData.w <= index)
    {
        return;
    }

    //�C���f�b�N�X���ȓ��Ȃ珈������
    //�O�p�`���\������C���f�b�N�X�̎w��--------------------------------------------
    uint firstVertIndex = index * 3;
    uint secondVertIndex = index * 3 + 1;
    uint thirdVertIndex = index * 3 + 2;

    uint uvFirstVertIndex = firstVertIndex;
    uint uvSecondVertIndex = secondVertIndex;
    uint uvThirdVertIndex = thirdVertIndex;
    //�O�p�`���\������C���f�b�N�X�̎w��--------------------------------------------

    //���_���W���烏�[���h���W�ɕϊ���----------------------------------------------
    float3 firstVertWorldPos;
    float3 secondVertWorldPos;
    float3 thirdVertWorldPos;

    firstVertWorldPos = GetPos(vertciesData[firstVertIndex],float3(0,0,0)).xyz;
    secondVertWorldPos = GetPos(vertciesData[secondVertIndex],float3(0,0,0)).xyz;
    thirdVertWorldPos = GetPos(vertciesData[thirdVertIndex],float3(0,0,0)).xyz;
    //���_���W���烏�[���h���W�ɕϊ�------------------------------------------------

    //�O�p�`���\�����郌�C--------------------------------------------
    const int RAY_MAX_NUM = 3;
    const int RAY_POS_MAX_NUM = 2;
    float3 triangleRay[RAY_MAX_NUM][RAY_POS_MAX_NUM];
    triangleRay[0][0] = firstVertWorldPos;
    triangleRay[0][1] = secondVertWorldPos;
    triangleRay[1][0] = secondVertWorldPos;
    triangleRay[1][1] = thirdVertWorldPos;
    triangleRay[2][0] = thirdVertWorldPos;
    triangleRay[2][1] = firstVertWorldPos;

    //�O�p�`���\�����郌�C--------------------------------------------

    //�d�S���W
    float3 triangleCentralPos = (firstVertWorldPos + secondVertWorldPos + thirdVertWorldPos) / 3.0f;

    //�O�p�`�̖ʐς��v�Z
    float triangleArea = CalucurateTriangleArea(firstVertWorldPos,secondVertWorldPos,thirdVertWorldPos);

 
    //�p�[�e�B�N���̔z�u--------------------------------------------
    const int PARTICLE_MAX_NUM = meshData.z;
    const int PER_PARTICLE_MAX_NUM = PARTICLE_MAX_NUM / 3;
    for(int rayIndex = 0; rayIndex < RAY_MAX_NUM; ++rayIndex)
    {
        //��ӂ̒���
        float3 distance = triangleRay[rayIndex][1] - triangleRay[rayIndex][0];

        for(int particleIndex = 0; particleIndex < PER_PARTICLE_MAX_NUM; ++particleIndex)
        {
            uint outputIndex = index * PARTICLE_MAX_NUM + rayIndex * PER_PARTICLE_MAX_NUM + particleIndex;
            float rate = RandVec3(outputIndex + 100,1,0).x;
            //�n�_�Z�o
            float3 startPos = triangleRay[rayIndex][0] + distance * rate;
            //�I�_�Z�o
            float3 endPos = triangleCentralPos;
            //��ӂ̂Ƃ���ʒu����d�S���W�܂ł̋���
            float3 resultDistance = endPos - startPos;

            //�p�[�e�B�N���̔z�u
            float3 resultPos;
            const int PARTICLE_MAX_BIAS = 100;
            const int RANDOM_NUMBER_BIAS = meshData.y;
            
            if(RandVec3(outputIndex,PARTICLE_MAX_BIAS,0).x <= RANDOM_NUMBER_BIAS)
            {
                //�G�b�W���ӂɕ΂点��
                rate = RandVec3(outputIndex + 1000,10,0).x / 100.0f;
                resultPos = startPos + resultDistance * rate;              
            }
            else
            {
                //�ʎ��ӂɕ΂点��
                rate = RandVec3(startPos.y * 10.0f + outputIndex + 10000,1,0).x;
                resultPos = startPos + resultDistance * rate;
            }

            ParticleData output;
            output.pos = resultPos;
            output.color = color;
            output.id = motherMatIndex;
            outputData.Append(output);            
            //outputData[outputIndex] = output;
        }
    }
    //�p�[�e�B�N���̔z�u--------------------------------------------
}