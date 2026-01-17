#include "BackGround.h"

void BackGround::Init(ID3D11ShaderResourceView* pTexture, int count, int xCount, float w, float h, float speed)
{
    m_pTexture = pTexture;
    m_Position = { 0.0f, 0.0f };    // 左上から
    m_Size = { 1280.0f, 720.0f };   // 画面全体サイズ
    m_Animator.Init(count, xCount, w, h, speed);
}

void BackGround::Draw(ID3D11DeviceContext* pContext, SpriteRenderer* pRenderer, DirectX::XMMATRIX viewProj)
{
    AnimFrame f = m_Animator.GetCurrentFrame();

    pRenderer->Draw(
        pContext,
        m_pTexture,
        m_Position.x, m_Position.y, // 表示位置 X, Y
        m_Size.x, m_Size.y,         // 表示サイズ W, H
        viewProj,
        f.x, f.y, f.w, f.h          //切り抜き位置 (UV)
    );
}

void BackGround::Update()
{
    m_Animator.Update(1.0f / 60.0f);
}
