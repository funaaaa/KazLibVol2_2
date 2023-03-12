
// 円周率
static const float PI = 3.141592653589f;

// 頂点情報
struct Vertex
{
    float3 Position;
    float3 Normal;
    float3 uv;
    uint boneIndex[4];
    float boneWeight[4];
};

// ペイロード
struct Payload
{
    float3 color_; // 色情報
    float impactAmount_; // 合計影響度
};

struct MyAttribute
{
    float2 barys;
};

// barysを計算
inline float3 CalcBarycentrics(float2 Barys)
{
    return float3(1.0 - Barys.x - Barys.y, Barys.x, Barys.y);
}

// 当たった位置の情報を取得する関数
Vertex GetHitVertex(MyAttribute attrib, StructuredBuffer<Vertex> vertexBuffer, StructuredBuffer<uint> indexBuffer, inout Vertex meshInfo[3])
{
    Vertex v = (Vertex) 0;
    float3 barycentrics = CalcBarycentrics(attrib.barys);
    uint vertexId = PrimitiveIndex() * 3; // Triangle List のため.

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
        
        // メッシュの情報を保存。
        meshInfo[index].Position = mul(float4(vertexBuffer[vtxIndex].Position, 1), ObjectToWorld4x3());
        meshInfo[index].Normal = normalize(mul(vertexBuffer[vtxIndex].Normal, (float3x3) ObjectToWorld4x3()));
        meshInfo[index].uv = vertexBuffer[vtxIndex].uv;
    }

    return v;
}