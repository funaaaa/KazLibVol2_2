
// �~����
static const float PI = 3.141592653589f;

// CHS�ł̎��ʗp�ϐ�
static const int CHS_IDENTIFICATION_INSTANCE_DEF = 0; // InstanceID �ʏ�̃I�u�W�F�N�g
static const int CHS_IDENTIFICATION_INSTNACE_AS = 1; // InstanceID ��C�U���p�̃I�u�W�F�N�g
static const int CHS_IDENTIFICATION_INSTANCE_TEXCOLOR = 2; // InstanceID �e�N�X�`���̐F�����̂܂ܕԂ��I�u�W�F�N�g
static const int CHS_IDENTIFICATION_INSTANCE_DEF_EMISSIVE = 3; // InstanceID �ʏ픽�˂ƃG�~�b�V�u
static const int CHS_IDENTIFICATION_INSTANCE_TEXCOLOR_REFLECTION_EMISSIVE = 4; // InstanceID �ʏ픽�˂ƃe�N�X�`���̐F���̂܂܂ƃG�~�b�V�u
static const int CHS_IDENTIFICATION_INSTANCE_LIGHT = 5; // instanceID ���C�g�p�I�u�W�F�N�g �e�N�X�`���̐F�����̂܂ܕԂ��BMissShader�œ����蔻������p����ׂɂ��g�p����B
static const int CHS_IDENTIFICATION_INSTANCE_REFRACTION = 6; // instanceID ���܂̏���
static const int CHS_IDENTIFICATION_INSTANCE_INVISIBILITY = 7; // instanceID ���C�e�B���O���`����s��Ȃ��I�u�W�F�N�g
static const int CHS_IDENTIFICATION_INSTANCE_DEF_GI = 8; // instanceID �ʏ�̏��� + GI���s���B
static const int CHS_IDENTIFICATION_INSTANCE_DEF_GI_TIREMASK = 9; // instanceID �ʏ�̏��� + GI + �^�C����
static const int CHS_IDENTIFICATION_INSTANCE_ALPHA = 10; // instanceID ������
static const int CHS_IDENTIFICATION_INSTANCE_ADD = 11; // instanceID ���Z����
static const int CHS_IDENTIFICATION_INSTANCE_DEF_TIREMASK = 12; // instanceID �ʏ�̏��� + �^�C����
static const int CHS_IDENTIFICATION_INSTANCE_DEF_TAILLAMP = 13; // instanceID �ʏ�̏��� + �G�~�b�V�u + �}�e���A���A���t�@
static const int CHS_IDENTIFICATION_INSTANCE_DEF_AO = 14; // instanceID �ʏ�̏��� + AO
static const int CHS_IDENTIFICATION_INSTANCE_DEF_TIREMASK_AO = 15; // instanceID �ʏ�̏��� + �^�C���� + AO

static const int CHS_IDENTIFICATION_RAYID_DEF = 100; // �f�t�H���g�̃��C
static const int CHS_IDENTIFICATION_RAYID_GI = 101; // GI�p�̃��C
static const int CHS_IDENTIFICATION_RAYID_RECLECTION = 102; // ���˗p�̃��C
static const int CHS_IDENTIFICATION_RAYID_COMPLETE_RECLECTION = 103; // ���S���˂̃��C
static const int CHS_IDENTIFICATION_RAYID_REFRACTION = 104; // ���܂̃��C
static const int CHS_IDENTIFICATION_RAYID_SHADOW = 105; // �e�p�̃��C
static const int CHS_IDENTIFICATION_RAYID_AO_SHADOW = 106; // AO�e�p�̃��C

// �J�����p�̒萔�o�b�t�@
struct CameraConstBufferData
{
    matrix mtxView; // �r���[�s��
    matrix mtxProj; // �v���W�F�N�V�����s��
    matrix mtxViewInv; // �t�r���[�s��
    matrix mtxProjInv; // �t�v���W�F�N�V�����s��
};

// �f�B���N�V���i�����C�g�p�萔�o�b�t�@
struct DirLightData
{
    float3 lightDir;
    int isActive;
    float3 lightColor;
    int seed;
};
// �|�C���g���C�g�p�萔�o�b�t�@
struct PointLightData
{
    float3 lightPos;
    float lightSize;
    float3 lightColor;
    float lightPower;
    int isActive;
    int isShadow;
    float2 pad;
};
// ���C�g�p�萔�o�b�t�@
struct LightConstBufferData
{
    DirLightData dirLight;
};
// �A���t�@�l�]���p�̒萔�o�b�t�@
struct AlphaData
{
    int instanceIndex_;
    int alpha_;
};
#define ALPHA_DATA_COUNT 60
struct AlphaConstBufferData
{
    AlphaData alphaData_[ALPHA_DATA_COUNT];
};

// �f�o�b�O�p���
struct DebugData
{
    int isDebugMeshInfo_;
    int debugMesnInfoX_;
    float2 pad_;
};

// �萔�o�b�t�@
struct ConstBufferData
{
    CameraConstBufferData camera;
    LightConstBufferData light;
    AlphaConstBufferData alphaData_;
    DebugData debug_;
};

static int MAP_NONE = 0;
static int MAP_NORMAL = 1;
static int MAP_SPECULAR = 2;
static int MAP_AO = 3;

// �}�e���A�����
struct Material
{
    float3 baseColor_;
    float metalness_;
    float specular_;
    float roughness_;
    int mapParam_; // �}�e���A���̒ǉ����B�ǉ��̃e�N�X�`�����ǂ��g�p���邩�𔻒f����B
    float alpha_;
};

// ���_���
struct Vertex
{
    float3 Position;
    float3 Normal;
    float2 uv;
    float2 subUV;
    float2 pad_;
};

// �y�C���[�h
struct Payload
{
    uint recursive_; // ������
    uint rayID_; // ���C��ID
    uint alphaCounter_; // �����A���t�@�̃I�u�W�F�N�g�ɓ���������
    uint isCullingAlpha_; // �����A���t�@�̃I�u�W�F�N�g�Ɉ��ȏ㓖�������玟����A���t�@�𖳌�������t���O�B
    float3 color_; // �F���
    float impactAmount_; // ���v�e���x
    float3 light_; // ���C�e�B���O�̐F���
    float ao_; // AO�̐F
    float3 denoiseMask_; // �f�m�C�Y�̃}�X�N�̐F���
    float roughnessOffset_;
    float3 emissive_; // �G�~�b�V�u�o�͗p
    float pad_;
};

struct MyAttribute
{
    float2 barys;
};

// barys���v�Z
inline float3 CalcBarycentrics(float2 Barys)
{
    return float3(1.0 - Barys.x - Barys.y, Barys.x, Barys.y);
}

// �e�̃��C���Ǝ�
bool ShootShadowRay(float3 Origin, float3 Direction, float TMax, RaytracingAccelerationStructure GRtScene)
{
    RayDesc rayDesc;
    rayDesc.Origin = Origin;
    rayDesc.Direction = Direction;
    rayDesc.TMin = 0.1f;
    rayDesc.TMax = TMax;

    Payload payloadData;
    payloadData.impactAmount_ = 1.0f;
    payloadData.rayID_ = CHS_IDENTIFICATION_RAYID_SHADOW;
    payloadData.recursive_ = 0;
    payloadData.ao_ = 0;
    payloadData.color_ = float3(0, 0, 0);
    payloadData.denoiseMask_ = float3(0, 0, 0);
    payloadData.light_ = float3(0, 0, 0);
    payloadData.isCullingAlpha_ = false;
    payloadData.alphaCounter_ = 0;
    payloadData.roughnessOffset_ = 1.0f;
    payloadData.pad_ = 0.0f;

    RAY_FLAG flags = RAY_FLAG_NONE;
    //flags |= RAY_FLAG_SKIP_CLOSEST_HIT_SHADER;
    flags |= RAY_FLAG_CULL_BACK_FACING_TRIANGLES;
    flags |= RAY_FLAG_FORCE_OPAQUE; // AnyHitShader���X�L�b�v
    
    // ???C?g????O?B
    uint rayMask = ~(0x08);

    TraceRay(
    GRtScene,
    flags,
    rayMask,
    0,
    1,
    1, // Miss�V�F�[�_�[�̃C���f�b�N�X���w�肷��B
    rayDesc,
    payloadData);

    return payloadData.impactAmount_;
}
float ShootShadowRayNoAH(float3 Origin, float3 Direction, float TMax, RaytracingAccelerationStructure GRtScene)
{
    RayDesc rayDesc;
    rayDesc.Origin = Origin;
    rayDesc.Direction = Direction;
    rayDesc.TMin = 0.1f;
    rayDesc.TMax = TMax;

    Payload payload;
    payload.impactAmount_ = 1.0f;
    payload.rayID_ = CHS_IDENTIFICATION_RAYID_SHADOW;
    payload.recursive_ = 0;
    payload.ao_ = 0;
    payload.color_ = float3(0, 0, 0);
    payload.denoiseMask_ = float3(0, 0, 0);
    payload.light_ = float3(0, 0, 0);
    payload.isCullingAlpha_ = false;
    payload.alphaCounter_ = 0;
    payload.roughnessOffset_ = 1.0f;
    payload.pad_ = 0.0f;

    RAY_FLAG flags = RAY_FLAG_NONE;
    flags |= RAY_FLAG_CULL_BACK_FACING_TRIANGLES;
    flags |= RAY_FLAG_FORCE_OPAQUE; // AnyHitShader���X�L�b�v
    
    // ???C?g????O?B
    uint rayMask = ~(0x08);

    TraceRay(
    GRtScene,
    flags,
    rayMask,
    0,
    1,
    1, // Miss�V�F�[�_�[�̃C���f�b�N�X���w�肷��B
    rayDesc,
    payload);

    return payload.impactAmount_;
}

// AO�p�̉e���C���ˏo
float ShootAOShadowRay(float3 Origin, float3 Direction, float TMax, RaytracingAccelerationStructure GRtScene)
{
    RayDesc rayDesc;
    rayDesc.Origin = Origin;
    rayDesc.Direction = Direction;
    rayDesc.TMin = 0.1f;
    rayDesc.TMax = TMax;

    Payload payload;
    payload.impactAmount_ = 1.0f;
    payload.rayID_ = CHS_IDENTIFICATION_RAYID_AO_SHADOW;
    payload.recursive_ = 0;
    payload.ao_ = 0;
    payload.color_ = float3(0, 0, 0);
    payload.denoiseMask_ = float3(0, 0, 0);
    payload.light_ = float3(0, 0, 0);
    payload.isCullingAlpha_ = false;
    payload.alphaCounter_ = 0;
    payload.roughnessOffset_ = 1.0f;
    payload.pad_ = 0.0f;

    RAY_FLAG flags = RAY_FLAG_NONE;
    flags |= RAY_FLAG_CULL_BACK_FACING_TRIANGLES;
    flags |= RAY_FLAG_FORCE_OPAQUE; // AnyHitShader���X�L�b�v
    
    // ???C?g????O?B
    uint rayMask = ~(0x08);

    TraceRay(
    GRtScene,
    flags,
    rayMask,
    0,
    1,
    1, // Miss�V�F�[�_�[�̃C���f�b�N�X���w�肷��B
    rayDesc,
    payload);

    return payload.impactAmount_;
}

// �l��ݒ肵�ă��C�𔭎ˁB
void ShootRay(uint RayID, float3 Origin, float3 Direction, inout Payload PayloadData, RaytracingAccelerationStructure GRtScene)
{
    RayDesc rayDesc;
    rayDesc.Origin = Origin;
    rayDesc.Direction = Direction;
    rayDesc.TMin = 0.1f;
    rayDesc.TMax = 300000.0f;

    // ???C??ID???p???C????B
    int rayID = PayloadData.rayID_;
    PayloadData.rayID_ = RayID;

    RAY_FLAG flags = RAY_FLAG_NONE;
    //flags |= RAY_FLAG_SKIP_CLOSEST_HIT_SHADER;
    flags |= RAY_FLAG_CULL_BACK_FACING_TRIANGLES;
    //flags |= RAY_FLAG_FORCE_NON_OPAQUE; // AnyHitShader�����s
    
    // ???C?g????O?B
    uint rayMask = 0xFF;

    TraceRay(
    GRtScene,
    flags,
    rayMask,
    0,
    1,
    0, // Miss�V�F�[�_�[�̃C���f�b�N�X���w�肷��B
    rayDesc,
    PayloadData);
    
    PayloadData.rayID_ = rayID;
    
}

// ���������ʒu�̏����擾����֐�
Vertex GetHitVertex(MyAttribute attrib, StructuredBuffer<Vertex> vertexBuffer, StructuredBuffer<uint> indexBuffer, inout Vertex meshInfo[3])
{
    Vertex v = (Vertex) 0;
    float3 barycentrics = CalcBarycentrics(attrib.barys);
    uint vertexId = PrimitiveIndex() * 3; // Triangle List �̂���.

    float weights[3] =
    {
        barycentrics.x, barycentrics.y, barycentrics.z
    };

    for (int index = 0; index < 3; ++index)
    {
        uint vtxIndex = indexBuffer[vertexId + index];
        float w = weights[index];
        v.Position += vertexBuffer[vtxIndex].Position * w;
        v.Normal += vertexBuffer[vtxIndex].Normal * w;
        v.uv += vertexBuffer[vtxIndex].uv * w;
        v.subUV += vertexBuffer[vtxIndex].subUV * w;
        
        // ���b�V���̏���ۑ��B
        meshInfo[index].Position = mul(float4(vertexBuffer[vtxIndex].Position, 1), ObjectToWorld4x3());
        meshInfo[index].Normal = normalize(mul(vertexBuffer[vtxIndex].Normal, (float3x3) ObjectToWorld4x3()));
        meshInfo[index].uv = vertexBuffer[vtxIndex].uv;
    }

    return v;
}

// �w��̒��_�̏Փ˂������b�V����ł̏d�S���W�����߂�B
float3 CalcVertexBarys(float3 HitVertex, float3 VertexA, float3 VertexB, float3 VertexC)
{
    
    float3 e0 = VertexB - VertexA;
    float3 e1 = VertexC - VertexA;
    float3 e2 = HitVertex - VertexA;
    float d00 = dot(e0, e0);
    float d01 = dot(e0, e1);
    float d11 = dot(e1, e1);
    float d20 = dot(e2, e0);
    float d21 = dot(e2, e1);
    float denom = 1.0 / (d00 * d11 - d01 * d01);
    float v = (d11 * d20 - d01 * d21) * denom;
    float w = (d00 * d21 - d01 * d20) * denom;
    float u = 1.0 - v - w;
    return float3(u, v, w);
    
}

// �������玝���Ă����֐��B
float Scale(float FCos)
{
    float x = 1.0 - FCos;
    return 0.25f * exp(-0.00287 + x * (0.459 + x * (3.83 + x * (-6.80 + x * 5.25))));
}
// �������玝���Ă����֐��B �V���Ƃ̌�_�����߂�B
float3 IntersectionPos(float3 Dir, float3 A, float Radius)
{
    float b = dot(A, Dir);
    float c = dot(A, A) - Radius * Radius;
    float d = max(b * b - c, 0.0);

    return A + Dir * (-b + sqrt(d));
}

// 3���_��UV�l����w����W�ł�U���iTangent�j�y��V���iBinormal�j���Z�o
void CalcTangentAndBinormal(float3 p0, float3 p1, float3 p2, float2 uv0, float2 uv1, float2 uv2, inout float3 outTangent, inout float3 outBinormal)
{
    // 5������3�������_��
    float3 CP0[3] =
    {
        float3(p0.x, uv0.x, uv0.y),
        float3(p0.y, uv0.x, uv0.y),
        float3(p0.z, uv0.x, uv0.y),
    };
    float3 CP1[3] =
    {
        float3(p1.x, uv1.x, uv1.y),
        float3(p1.y, uv1.x, uv1.y),
        float3(p1.z, uv1.x, uv1.y),
    };
    float3 CP2[3] =
    {
        float3(p2.x, uv2.x, uv2.y),
        float3(p2.y, uv2.x, uv2.y),
        float3(p2.z, uv2.x, uv2.y),
    };

    // ���ʃp�����[�^����UV�����W�Z�o
    float U[3], V[3];
    for (int i = 0; i < 3; ++i)
    {
        float3 V1 = CP1[i] - CP0[i];
        float3 V2 = CP2[i] - CP1[i];
        float3 ABC;
        ABC = cross(V1, V2);

        if (ABC.x == 0.0f)
        {
            return;
        }
        U[i] = -ABC.y / ABC.x;
        V[i] = -ABC.z / ABC.x;
    }

    outTangent.x = U[0];
    outTangent.y = U[1];
    outTangent.z = U[2];

    outBinormal.x = V[0];
    outBinormal.y = V[1];
    outBinormal.z = V[2];
    
    // ���K��
    outTangent = normalize(outTangent);
    outBinormal = normalize(outBinormal);
    
}