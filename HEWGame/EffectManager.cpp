#include "EffectManager.h"
void EffectManager::Init()
{
    // 必要ならここで配列のクリアなど
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

void EffectManager::LoadEffectTexture(EffectType type, const char* filename, ID3D11Device* device)
{
    ID3D11ShaderResourceView* tex = m_pResourceManager->LoadTexture(filename, m_pRenderer->GetDevice());
    m_TextureMap[type] = tex;
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

void EffectManager::Play(EffectType type, float x, float y)
{
    // 空いているエフェクトを探す
    for (int i = 0; i < MAX_EFFECTS; i++)
    {
        if (!m_Effects[i].IsActive())
        {
            // テクスチャを取得
            ID3D11ShaderResourceView* tex = m_TextureMap[type];
            if (!tex) return;

            // 発生させる (Texture, x, y, speedX, speedY, lifeTime, scale)
            m_Effects[i].Init(m_TextureMap[type], x, y, 100.0f, 18, 6, 320.0f, 320.0f, 0.05f);
            break;
        }
    }
}