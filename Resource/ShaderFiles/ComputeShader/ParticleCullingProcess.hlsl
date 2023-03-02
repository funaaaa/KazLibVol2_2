#include"../ShaderHeader/KazMathHeader.hlsli"
#include"../ShaderHeader/GPUParticle.hlsli"

struct OutputData
{
    matrix mat;    
    float4 color;
};


cbuffer RootConstants : register(b0)
{    
    matrix viewProjection;
};

//����
RWStructuredBuffer<GPUParticleInput> worldMatData : register(u0);

AppendStructuredBuffer<OutputData>drawData:register(u1);

//���[���h�s��̍��W��F����J�����O�������s���A�`��p�̃o�b�t�@�ɕϊ����鏈��
[numthreads(1024, 1, 1)]
void CSmain(uint3 groupId : SV_GroupID, uint groupIndex : SV_GroupIndex,uint3 groupThreadID : SV_GroupThreadID)
{
    uint index = groupThreadID.x;
    index += 1024 * groupId.x;

    matrix worldMat = worldMatData[index].worldMat;
    float4 color = worldMatData[index].color;

    OutputData outputMat;
    outputMat.mat = mul(viewProjection,worldMat);
    outputMat.color = color;
    drawData.Append(outputMat);
}
