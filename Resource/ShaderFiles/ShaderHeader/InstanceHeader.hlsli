#define INSTANCE_DATA_MAX 1

struct InstanceOutPut
{
    float4 svpos : SV_POSITION;
    float3 normal : NORMAL; //�@���x�N�g��
    float2 uv : TEXCOORD; //uv�l
    uint id : SV_InstanceID;
};

struct InstanceDepthOutPut
{
    float4 svpos : SV_POSITION;
    float3 normal : NORMAL; //�@���x�N�g��
    float2 uv : TEXCOORD; //uv�l
    float4 worldPos : POSITION;
    uint id : SV_InstanceID;
};


struct InstanceMat
{
    matrix matData;
};

struct InstanceConstBufferData
{
    float4 color; //�F
    matrix mat; //3D�ϊ��s��
};

struct MaterialData
{
    float3 m_ambient;
    float3 m_diffuse;
    float3 m_specular;
    float m_alpha;
};

struct LineInstanceData
{
    float4 startPos;
    float4 endPos;
    float4 color;
};


RWStructuredBuffer<InstanceMat> uavMatData : register(u1);

//�s����
cbuffer instanceConstBuff : register(b0)
{
    InstanceConstBufferData constData[INSTANCE_DATA_MAX];
};

RWStructuredBuffer<InstanceConstBufferData> uavMatColorData : register(u0);

cbuffer instanceLineConstBuff : register(b0)
{
    LineInstanceData instaceLineData[INSTANCE_DATA_MAX];
};

//�}�e���A�����
cbuffer instanceConstBuffMaterial : register(b0)
{
    MaterialData materialData;
};

struct InstanceIn
{
    float4 svpos : SV_POSITION; //�V�X�e���p���_���W
    float3 normal : NORMAL; //�@���x�N�g��
    float2 uv : TEXCOORD; //uv�l
    float id : SV_InstanceID;
};

struct InstancePosOut
{
    float4 svpos : SV_POSITION;
    uint id : SV_InstanceID;
};

struct InstancePosNormalColorOut
{
    float4 svpos : SV_POSITION;
    float3 normal : NORMAL; //�@���x�N�g��
    float4 color : COLOR;
};

struct lineData
{
    float4 pos[2];
};

cbuffer lineVertexData : register(b1)
{
    lineData VertexLine[INSTANCE_DATA_MAX];
};

