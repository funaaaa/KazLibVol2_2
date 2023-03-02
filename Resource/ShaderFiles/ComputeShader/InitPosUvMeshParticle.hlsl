#include"../ShaderHeader/KazMathHeader.hlsli"
#include"../ShaderHeader/GPUParticle.hlsli"

struct InputData
{
    float3 pos;
    float2 uv;
};

//���_���
RWStructuredBuffer<float3> vertciesData : register(u0);
RWStructuredBuffer<float2> uvData : register(u1);

//�o��
AppendStructuredBuffer<ParticleData> outputData : register(u2);

Texture2D<float4> tex : register(t0);
SamplerState smp : register(s0);

cbuffer CommonBuffer: register(b0)
{
    //x vertNum, y bias,z perTriangleNum,w faceCountNum
    uint4 meshData;
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
    InputData firstVertWorldPos;
    InputData secondVertWorldPos;
    InputData thirdVertWorldPos;

    firstVertWorldPos.pos = GetPos(vertciesData[firstVertIndex].xyz,float3(0,0,0));
    secondVertWorldPos.pos = GetPos(vertciesData[secondVertIndex].xyz,float3(0,0,0));
    thirdVertWorldPos.pos = GetPos(vertciesData[thirdVertIndex].xyz,float3(0,0,0));
    firstVertWorldPos.uv = uvData[uvFirstVertIndex];
    secondVertWorldPos.uv = uvData[uvSecondVertIndex];
    thirdVertWorldPos.uv = uvData[uvThirdVertIndex];
    //���_���W���烏�[���h���W�ɕϊ�------------------------------------------------

    //�O�p�`���\�����郌�C--------------------------------------------
    const int RAY_MAX_NUM = 3;
    const int RAY_POS_MAX_NUM = 2;
    float3 triangleRay[RAY_MAX_NUM][RAY_POS_MAX_NUM];
    triangleRay[0][0] = firstVertWorldPos.pos.xyz;
    triangleRay[0][1] = secondVertWorldPos.pos.xyz;
    triangleRay[1][0] = secondVertWorldPos.pos.xyz;
    triangleRay[1][1] = thirdVertWorldPos.pos.xyz;
    triangleRay[2][0] = thirdVertWorldPos.pos.xyz;
    triangleRay[2][1] = firstVertWorldPos.pos.xyz;

    //�O�p�`���\�����郌�C--------------------------------------------

    //�d�S���W
    float3 triangleCentralPos = (firstVertWorldPos.pos.xyz + secondVertWorldPos.pos.xyz + thirdVertWorldPos.pos.xyz) / 3.0f;

    //�O�p�`�̖ʐς��v�Z
    float triangleArea = CalucurateTriangleArea(firstVertWorldPos.pos.xyz,secondVertWorldPos.pos.xyz,thirdVertWorldPos.pos.xyz);

 
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

            //���W����UV�����------------------------------------------------------------------------------------------------
            float3 uvw;
            uvw.x = CalucurateUVW(firstVertWorldPos.pos.xyz,secondVertWorldPos.pos.xyz,resultPos,triangleArea);
            uvw.y = CalucurateUVW(secondVertWorldPos.pos.xyz,thirdVertWorldPos.pos.xyz,resultPos,triangleArea);
            uvw.z = CalucurateUVW(thirdVertWorldPos.pos,firstVertWorldPos.pos,resultPos,triangleArea);
            
            float2 firstVec;
            firstVec.x = firstVertWorldPos.uv.x * uvw.x;
            firstVec.y = firstVertWorldPos.uv.y * uvw.x;

            float2 secondVec;
            secondVec.x = secondVertWorldPos.uv.x * uvw.y;
            secondVec.y = secondVertWorldPos.uv.y * uvw.y;

            float2 thirdVec;
            thirdVec.x = thirdVertWorldPos.uv.x * uvw.z;
            thirdVec.y = thirdVertWorldPos.uv.y * uvw.z;

            float2 uv = firstVec + secondVec + thirdVec;

            ParticleData output;
            output.pos = resultPos;
            output.color = tex.SampleLevel(smp,uv,0);
            output.id = motherMatIndex;                
            outputData.Append(output);
            //���W����UV�����------------------------------------------------------------------------------------------------
        }
    }
    //�p�[�e�B�N���̔z�u--------------------------------------------
}