#pragma once
#include "Effect.h"
#include <vector>
#include <map>
#include "ResourceManager.h"

#define MAX_EFFECTS 100

// エフェクトごとの設定データ
struct EffectDef
{
    EffectType type;
    const char* filename;   // ファイル名
    int frameCount;         // 総コマ数
    int divX;               // 横分割数
    float texW;             // 画像全体の幅
    float texH;             // 画像全体の高さ
    float speed;            // アニメーション速度
    float offsetX;          // 中心からのズレX (右向き基準)
    float offsetY;          // 中心からのズレY (足元ならプラス)
    float scale;            // サイズ倍率
    float startTexX;
    float startTexY;
};
class EffectManager
{
private:
    // エフェクトの実体たち
    Effect m_Effects[MAX_EFFECTS];
    std::map<EffectType, ID3D11ShaderResourceView*> m_TextureMap;

    //設定データのリスト
    std::map<EffectType, EffectDef> m_EffectDefs;
    
public:
    EffectManager() {}
    ~EffectManager() { Uninit(); }

    void Init();
    void Uninit();
    void Update();
    void Draw(ID3D11DeviceContext* context, SpriteRenderer* spriteRenderer, DirectX::XMMATRIX viewProj);

    // テクスチャ読み込み
    void LoadEffectTexture(EffectType type, const char* filename, ID3D11Device* device, ResourceManager* mgr);

    // エフェクト発生（種類, 場所）
    void Play(EffectType type, float x, float y,bool flipX = false,float angle = 0.0f);
};