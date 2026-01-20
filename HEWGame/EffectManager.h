#pragma once
#include "Effect.h"
#include <vector>
#include <map>

// エフェクトの最大数
#define MAX_EFFECTS 100

class EffectManager
{
private:
    // エフェクトの実体たち
    Effect m_Effects[MAX_EFFECTS];

    // 読み込んだテクスチャリスト (種類 -> テクスチャ)
    std::map<EffectType, ID3D11ShaderResourceView*> m_TextureMap;

public:
    EffectManager() {}
    ~EffectManager() { Uninit(); }

    void Init();
    void Uninit();
    void Update();
    void Draw(ID3D11DeviceContext* context, SpriteRenderer* spriteRenderer, DirectX::XMMATRIX viewProj);

    // テクスチャ読み込み
    void LoadEffectTexture(EffectType type, const char* filename, ID3D11Device* device);

    // エフェクト発生（種類, 場所）
    void Play(EffectType type, float x, float y);
};