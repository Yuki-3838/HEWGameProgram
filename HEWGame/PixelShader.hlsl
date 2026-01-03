Texture2D g_texture : register(t0);
SamplerState g_sampler : register(s0);

struct VS_OUTPUT
{
    float4 Pos : SV_POSITION;
    float2 Tex : TEXCOORD;
};

float4 main(VS_OUTPUT Input) : SV_Target
{
    // テクスチャから色を抽出し、透明度も含めて出力
    float4 color = g_texture.Sample(g_sampler, Input.Tex);
    return color;
}