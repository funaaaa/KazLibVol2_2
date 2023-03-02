#include"../ShaderHeader/KazMathHeader.hlsli"

struct SceneConstantBuffer
{
    float4 velocity;
    float4 offset;
    float4 color;
    float4x4 projection;
    float4 padding[9];
};

//https://docs.microsoft.com/en-us/windows/win32/api/d3d12/ns-d3d12-d3d12_draw_arguments
struct D3D12_DRAW_ARGUMENTS
{
    uint VertexCountPerInstance;
    uint InstanceCount;
    uint StartVertexLocation;
    uint StartInstanceLocation;
};


struct IndirectCommand
{
    uint64_t cbvAddress;    
    D3D12_DRAW_ARGUMENTS drawArguments;
};

struct OutputData
{
    matrix mat;
    float4 color;
};

struct UpdateData
{
    float4 pos;
    float4 vel;
    float4 color;
    int timer;
};

cbuffer RootConstants : register(b0)
{
    matrix view;        
    matrix projection;
    float4 emmittPos;
    uint increSize;     //�s��ƐF���o�͂���\���̂̃T�C�Y
    uint64_t gpuAddress; //�s��ƐF���o�͂���o�b�t�@��GPU�A�h���X
    int seed;
};

//�o��
AppendStructuredBuffer<OutputData> matrixData : register(u0);
//�X�V
RWStructuredBuffer<UpdateData> updateData : register(u1);

static const int NUM = 1;

[numthreads(NUM, 1, 1)]
void CSmain(uint3 groupId : SV_GroupID, uint groupIndex : SV_GroupIndex)
{
    uint index = (groupId.x * NUM) + groupIndex;
    //uint index = 9000;

    float4 initPos = emmittPos;

    //��������p�[�e�B�N���̔��f-------------------------
    //�ړ��ʂƐ������Ԃ̏�����
    if(updateData[index].timer <= 0)
    {
        updateData[index].timer = Rand(index,120,0);
        updateData[index].pos = initPos;
        updateData[index].color = float4(Rand(index,255,0),Rand(index,255,0),Rand(index,255,0),1.0f);
        updateData[index].vel = float4(Rand(index,2,0),0.0f,0.0f,0.0f);
    }    
    //��������p�[�e�B�N���̔��f-------------------------

    //�X�V
    if(0 < updateData[index].timer)
    {
        updateData[index].pos += updateData[index].vel;
        updateData[index].vel.y -= 0.1f;
        --updateData[index].timer;
    }


    //�s��v�Z-------------------------
    matrix pMatTrans = Translate(updateData[index].pos.xyz);
    matrix pMatRot = Rotate(float3(0.0f,0.0f,0.0f));
    matrix pMatScale = Scale(float3(1.0f, 1.0f, 1.0f));
    
    matrix pMatWorld = MatrixIdentity();
    pMatWorld = mul(pMatScale, pMatWorld);
    pMatWorld = mul(pMatRot, pMatWorld);
    pMatWorld = mul(pMatTrans, pMatWorld);
    //�s��v�Z-------------------------
    
    
    //�o�͗p-------------------------
    OutputData outputMat;
    matrix lView = view;
    matrix lproj = projection;

    outputMat.mat = MatrixIdentity();
    outputMat.mat = mul(pMatWorld,outputMat.mat);
    outputMat.mat = mul(lView,    outputMat.mat);
    outputMat.mat = mul(lproj,    outputMat.mat);
    outputMat.color = updateData[index].color;
    matrixData.Append(outputMat);    
    //�o�͗p-------------------------

}