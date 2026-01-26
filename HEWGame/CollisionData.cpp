#include "CollisionData.h"

CollisionData::CollisionData()
{
    m_Position = { 0.0f, 0.0f };
    m_Size = { 0.0f, 0.0f };
    m_pTexture = nullptr;
}

CollisionData::~CollisionData()
{
}

void CollisionData::Init(ID3D11ShaderResourceView* pTexture, float x, float y, float width, float height)
{
    m_pTexture = pTexture;
    m_Position = { x, y };
    m_Size = { width, height };
}

void CollisionData::UpdatePos(float parentX, float parentY, float offsetX, float offsetY)
{
    // 親の座標 + オフセット
    m_Position.x = parentX + offsetX;
    m_Position.y = parentY + offsetY;
}

void CollisionData::Draw(ID3D11DeviceContext* pContext, SpriteRenderer* pSR, DirectX::XMMATRIX viewProj)
{
    // 画像とレンダラーがある場合のみ描画
    if (m_pTexture && pSR)
    {
        // SpriteRendererのDrawを直接呼び出す
        pSR->Draw(
            pContext,
            m_pTexture,
            m_Position.x,
            m_Position.y,
            m_Size.x,
            m_Size.y,
            viewProj
        );
    }
}
