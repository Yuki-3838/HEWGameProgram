#include "GameObject.h"

GameObject::GameObject()
{
    m_Position.x = 0.0f;
    m_Position.y = 0.0f;
    m_Size.x = 64.0f;
    m_Size.y = 64.0f;
    m_pTexture = nullptr;
}

GameObject::~GameObject()
{

}
void GameObject::Init(ID3D11ShaderResourceView* pTexture)
{
    m_pTexture = pTexture;
}


void GameObject::Draw(ID3D11DeviceContext* pContext, SpriteRenderer* pSR, DirectX::XMMATRIX viewProj)
{
    // テクスチャとレンダラーが存在する場合のみ描画を実行
    if (m_pTexture && pSR)
    {
        // SpriteRenderer::Draw の引数順序:
        // (context, texture, x, y, width, height, matrix)
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