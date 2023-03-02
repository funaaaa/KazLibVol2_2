#include"../ShaderHeader/KazMathHeader.hlsli"
#include"../ShaderHeader/NoiseShaderHeader.hlsli"

struct EmitterData
{
    float4 emittPos;
};

struct UpdateData
{
    float4 pos;
    int timer;
};

struct OutputData
{
    matrix mat;
    float4 color;
};

cbuffer RootConstants : register(b0)
{
    matrix view;
    matrix projection;
    matrix billBoard;
};

//����
AppendStructuredBuffer<OutputData> MatrixArrayBuff : register(u0);
//�X�V
RWStructuredBuffer<UpdateData> ParticleArrayBuff : register(u1);
RWStructuredBuffer<EmitterData> EmitterArrayBuff : register(u2);

static const int THREAD_MAX = 1;

[numthreads(THREAD_MAX, THREAD_MAX, THREAD_MAX)]
void CSmain(uint3 groupId : SV_GroupID, uint groupIndex : SV_GroupIndex,uint3 groupThreadID : SV_GroupThreadID)
{
    uint index = (groupThreadID.y * THREAD_MAX) + groupThreadID.x + ((THREAD_MAX * THREAD_MAX) * groupThreadID.z);
    index += (THREAD_MAX * THREAD_MAX * THREAD_MAX) * groupId.x;

    UpdateData particleData = ParticleArrayBuff[index];
    EmitterData emitterData = EmitterArrayBuff[index];

    //�p�[�e�B�N���̏�����
    if(particleData.timer == -1)
    {
        particleData.pos = emitterData.emittPos;
        particleData.timer = 60;
    }

    //�p�[�e�B�N���̍X�V
    if(0 <= particleData.timer)
    {
        float3 vel = CurlNoise(particleData.pos.xyz,particleData.timer);
        particleData.pos.xyz += vel.xyz;
        --particleData.timer;
    }

    //�p�[�e�B�N���̏o��
    //�o�͗p-------------------------
    OutputData outputMat;
    matrix lView = view;
    matrix lproj = projection;
    matrix world = CalucurateWorldMat(particleData.pos.xyz,float3(1,1,1),float3(0,0,0), billBoard);

    outputMat.mat = MatrixIdentity();
    outputMat.mat = mul(world,    outputMat.mat);
    outputMat.mat = mul(lView,    outputMat.mat);
    outputMat.mat = mul(lproj,    outputMat.mat);
    outputMat.color = float4(1.0f,float(particleData.timer) / 60.0f,0.0f,1.0f); 
    MatrixArrayBuff.Append(outputMat);
    //�o�͗p------------------------- 

    ParticleArrayBuff[index] = particleData;
    EmitterArrayBuff[index] = emitterData;
}
