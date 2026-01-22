#include "EffectManager.h"
void EffectManager::Init()
{
    m_EffectDefs[EffectType::Smoke] = 
    {
    EffectType::Smoke,              // 1. エフェクトの種類
    "asset/texture/testSP.png",     // 2. 画像ファイル名
    18,                             // 3. 総コマ数 (全部で何コマあるか)
    6,                              // 4. 横の分割数 (横に何コマ並んでいるか)
    320.0f,                         // 5. 1コマの幅 (TexW)
    320.0f,                         // 6. 1コマの高さ (TexH)
    0.05f,                          // 7. アニメーション速度 (1コマの秒数)
    0.0f,                          // 8. 発生位置の横ズレ (OffsetX: プレイヤー中心から横にどれだけずらすか)
    0.0f,                          // 9. 発生位置の縦ズレ (OffsetY: 足元に合わせるための調整)
    0.5f,                           // 10. サイズ倍率 
    320.0f,                         // 11. 画像の読み取り開始位置X (StartTexX: 画像の右側を使うため320から開始)
    0.0f                            // 12. 画像の読み取り開始位置Y (StartTexY)
    };
}

void EffectManager::Uninit()
{
    // テクスチャの解放
    for (auto& pair : m_TextureMap)
    {
        if (pair.second) pair.second->Release();
    }
    m_TextureMap.clear();
}

void EffectManager::LoadEffectTexture(EffectType type, const char* filename, ID3D11Device* device, ResourceManager* mgr)
{
    if (mgr)
    {
        ID3D11ShaderResourceView* tex = mgr->LoadTexture(filename, device);
        m_TextureMap[type] = tex;
    }
}

void EffectManager::Update()
{
    for (int i = 0; i < MAX_EFFECTS; i++)
    {
        m_Effects[i].Update(1.0f / 60.0f);
    }
}

void EffectManager::Draw(ID3D11DeviceContext* context, SpriteRenderer* spriteRenderer, DirectX::XMMATRIX viewProj)
{
    for (int i = 0; i < MAX_EFFECTS; i++)
    {
        m_Effects[i].Draw(context, spriteRenderer, viewProj);
    }
}


Effect* EffectManager::Play(EffectType type, float x, float y, bool flipX, float angle)
{
    if (m_EffectDefs.count(type) == 0) return nullptr;
    EffectDef& def = m_EffectDefs[type];

    if (m_TextureMap.count(type) == 0) return nullptr;
    ID3D11ShaderResourceView* tex = m_TextureMap[type];

    for (int i = 0; i < MAX_EFFECTS; i++)
    {
        if (!m_Effects[i].IsActive())
        {
            // --- 座標の計算 ---
            // プレイヤーが反転(isFlip)していたら、X方向のズレを逆にする
            float useOffsetX = flipX ? -def.offsetX : def.offsetX;

            float finalX = x + useOffsetX;
            float finalY = y + def.offsetY;

            // 初期化 (引数: tex, x, y, scale, count, divX, w, h, speed, flip, angle)
            // ※Effect.h/cppのInit引数に合わせて修正済みと仮定
            m_Effects[i].Init(
                tex, finalX, finalY, def.scale,
                def.frameCount, def.divX, def.texW, def.texH, def.speed,
                flipX, angle, def.startTexX, def.startTexY
            );
            return &m_Effects[i];
        }
    }
    return nullptr;
}