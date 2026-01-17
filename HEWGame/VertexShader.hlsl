// 定数バッファ (C++側の SpriteConstantBufferData と一致させる)
cbuffer SpriteConstantBuffer : register(b0)
{
    float4x4 World; // スプライトの位置・サイズ
    float4x4 ViewProjection; // カメラの行列
    float4 UVTransform; // UV変換
    
};

struct VS_INPUT
{
    float3 Pos : POSITION;
    float2 Tex : TEXCOORD;
};

struct VS_OUTPUT
{
    float4 Pos : SV_POSITION;
    float2 Tex : TEXCOORD;
};

VS_OUTPUT main(VS_INPUT Input)
{
    VS_OUTPUT Output;
    
    // 1. ローカル座標をワールド座標へ、さらにカメラ空間へ変換
    float4 pos = float4(Input.Pos, 1.0f);
    pos = mul(pos, World);
    pos = mul(pos, ViewProjection);
    
    Output.Pos = pos;
    // 2. UV座標の変換
    Output.Tex = Input.Tex*UVTransform.zw + UVTransform.xy;
    
    return Output;
}