
//BB���`�Â���ׂ̃f�[�^
struct BoundingBoxData
{
    float3 minPos;
    float3 maxPos;
};

//�p�[�e�B�N���̓����蔻����
struct ParticleHitBoxData
{
    float3 pos;
    float4 color;
    uint meshID;
    uint3 id;
};

struct MeshHitBox
{
    float3 pos;
    uint meshID;
    uint3 id;
};

struct MeshSphereHitData
{
    float3 meshPos;
    uint meshID;
    uint3 id;
    //�Ώۋ��̍��W
    float3 circlePos;
};

struct HitIDData
{
    uint meshID;
    uint3 id;
};