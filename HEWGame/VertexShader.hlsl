//--------------------------------------------------------------------------------------
// 頂点シェーダー
//--------------------------------------------------------------------------------------

// 頂点のデータを表す構造体（受け取り用）
struct VS_IN
{
    float4 pos : POSITION;
    float4 col : COLOR0;
    float2 tex : TEX; //UV座標
};

//グローバル変数の宣言
//定数バッファ受け取り用
cbuffer ConstBuffer : register(b0)
{
    //頂点カラー
    float4 vertexColor;
    //UV行列
    matrix matrixTex;
    //プロジェクション変換行列
    matrix matrixProj;
    //ワールド変換行列
    matrix matrixWorld;
}

// 頂点のデータを表す構造体（送信用） 
struct VS_OUT
{
    float4 pos : SV_POSITION;
    float4 col : COLOR0;
    float2 tex : TEXCOORD; //UV座標
};
 
// 頂点シェーダーのエントリポイント 
VS_OUT main(VS_IN input)
{
    VS_OUT output;
 
    //ワールド変換行列を頂点座標に掛けて、移動、回転、拡大縮小する
    output.pos = mul(input.pos,matrixWorld);
    //頂点座標に投影行列をかけて、平面上の座標にする
    output.pos = mul(output.pos, matrixProj);
    //頂点カラーをセットする
    output.col = input.col * vertexColor;
    
    //UV座標を移動させる
    float4 uv;
    uv.xy = input.tex;//行列掛け算のためfloat4型に移す
    uv.z = 0.0f;
    uv.w = 1.0f;
    uv = mul(uv, matrixTex);//UV座標と移動行列を掛け算
    output.tex = uv.xy;//掛け算の結果を送信用変数にセット

    return output;
}