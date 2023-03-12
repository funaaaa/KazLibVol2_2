#include "RaytracingShaderHeader.hlsli"

// TLAS
RaytracingAccelerationStructure gRtScene : register(t0);

// 各リソース等
StructuredBuffer<uint> indexBuffer : register(t0, space1);
StructuredBuffer<Vertex> vertexBuffer : register(t1, space1);
Texture2D<float4> texture : register(t2, space1);
// サンプラー
SamplerState smp : register(s0, space1);

// 出力先UAV
RWTexture2D<float4> output : register(u0);
RWTexture2D<float4> gBuffer0 : register(u1);
RWTexture2D<float4> gBuffer1 : register(u2);
RWTexture2D<float4> renderUAV : register(u3);

// RayGenerationシェーダー
[shader("raygeneration")]
void mainRayGen()
{

    uint2 launchIndex = DispatchRaysIndex().xy;
    float2 dims = float2(DispatchRaysDimensions().xy);

    // レイの設定
    RayDesc rayDesc;
    rayDesc.Origin = gBuffer0[launchIndex].xyz + gBuffer1[launchIndex].xyz * 1.0f;

    rayDesc.Direction = gBuffer1[launchIndex].xyz;
    rayDesc.TMin = 0;
    rayDesc.TMax = 300000;

    // ペイロードの設定
    Payload payloadData;
    payloadData.impactAmount_ = 1.0f;
    payloadData.color_ = renderUAV[launchIndex].xyz;

    // TransRayに必要な設定を作成
    uint rayMask = 0xFF;
    
    RAY_FLAG flag = RAY_FLAG_NONE;
    flag |= RAY_FLAG_CULL_BACK_FACING_TRIANGLES;
    
    // レイを発射
    TraceRay(
    gRtScene, // TLAS
    flag, // 衝突判定制御をするフラグ
    rayMask, // 衝突判定対象のマスク値
    0, // ray index
    1, // MultiplierForGeometryContrib
    0, // miss index
    rayDesc,
    payloadData);
    
    float3 color = payloadData.color_;
    
    if (color.y == 1)
    {
        color = renderUAV[launchIndex.xy].xyz;
    }
    else
    {
        color = payloadData.color_ / 2.0f;

    }

    // 結果格納
    output[launchIndex.xy] = float4(color, 1);

}

// missシェーダー レイがヒットしなかった時に呼ばれるシェーダー
[shader("miss")]
void mainMS(inout Payload PayloadData)
{
    
    PayloadData.color_ = float3(0, 1, 0);
}

// closesthitシェーダー レイがヒットした時に呼ばれるシェーダー
[shader("closesthit")]

    void mainCHS
    (inout
    Payload PayloadData, MyAttribute
    attrib)
{
    
    Vertex meshInfo[3];
    Vertex vtx = GetHitVertex(attrib, vertexBuffer, indexBuffer, meshInfo);
    uint instanceID = InstanceID();
    float3 worldPos = mul(float4(vtx.Position, 1), ObjectToWorld4x3());
    float3 worldNormal = normalize(mul(vtx.Normal, (float3x3) ObjectToWorld4x3()));
    float3 normalMap = worldNormal;

    // テクスチャの色を取得。
    float4 texColor = (float4) texture.SampleLevel(smp, vtx.uv.xy, 0);
    
    PayloadData.color_ = texColor.xyz;
    
}

// 影用CHS 使用していない。
[shader("closesthit")]

    void shadowCHS
    (inout
    Payload payload, MyAttribute
    attrib)
{
}

// AnyHitShader
[shader("anyhit")]

    void mainAnyHit
    (inout
    Payload payload, MyAttribute
    attrib)
{
}