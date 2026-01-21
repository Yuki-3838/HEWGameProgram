#include "EffectManager.h"
void EffectManager::Init()
{
    m_EffectDefs[EffectType::Smoke] = {
        EffectType::Smoke, "asset/texture/testSP.png",
        18, 6, 320.0f, 320.0f, 0.05f,
        40.0f, 64.0f, 1.0f  // ★右に40、下に64ずらす
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


void EffectManager::Play(EffectType type, float x, float y, bool flipX, float angle)
{
    if (m_EffectDefs.count(type) == 0) return;
    EffectDef& def = m_EffectDefs[type];

    if (m_TextureMap.count(type) == 0) return;
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
                flipX, angle
            );
            break;
        }
    }
}