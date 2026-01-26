#include "BackGround.h"
#include <cmath>

void BackGround::Init(ID3D11ShaderResourceView* pTexture, int count, int xCount, float w, float h, float speed)
{
    // アニメーション表示用
    m_pTexture = pTexture;
    m_IsParallax = false;

    // Animator 初期化（既存プロジェクトで期待される引数順に合わせる）
    m_Animator.Init(count, xCount, w, h, speed, 0.0f);

    // 初期表示位置・サイズ（TitleScene が SetSize することがある）
    m_Position.x = 0.0f;
    m_Position.y = 0.0f;
    m_Size.x = w;
    m_Size.y = h;

    m_ParallaxFactor = 0.0f;
    m_OffsetX = 0.0f;
}

void BackGround::Init(ID3D11ShaderResourceView* pTexture, float width, float height, float parallaxFactor)
{
    // パララックス背景用
    m_pTexture = pTexture;
    m_IsParallax = true;

    m_Position.x = 0.0f;
    m_Position.y = 0.0f;
    m_Size.x = width;
    m_Size.y = height;

    m_ParallaxFactor = parallaxFactor;
    m_OffsetX = 0.0f;
}

void BackGround::Update()
{
    m_Animator.Update(1.0f / 60.0f);
}

void BackGround::Update(const DirectX::XMFLOAT2& cameraPos)
{
    // パララックス更新
    if (m_Size.x == 0.0f) return;
    float raw = -cameraPos.x * m_ParallaxFactor;
    float mod = std::fmod(raw, m_Size.x);
    if (mod > 0.0f) mod -= m_Size.x;
    m_OffsetX = mod;
    // 必要なら Y 方向のパララックスもここで制御
}

void BackGround::Draw(ID3D11DeviceContext* pContext, SpriteRenderer* pRenderer, DirectX::XMMATRIX viewProj)
{
    AnimFrame f = m_Animator.GetCurrentFrame();

    if (m_IsParallax)
    {
        // 横ラップ描画（-1..1）で連続スクロール
        for (int i = -1; i <= 1; ++i)
        {
            float drawX = m_OffsetX + i * m_Size.x + m_Position.x;
            pRenderer->Draw(
                pContext,
                m_pTexture,
                drawX, m_Position.y,
                m_Size.x, m_Size.y,
                viewProj,
                f.x, f.y, f.w, f.h,
                0.0f,
                false
            );
        }
    }
    else
    {
        // アニメ表示（単一フレーム）
        pRenderer->Draw(
            pContext,
            m_pTexture,
            m_Position.x, m_Position.y,
            m_Size.x, m_Size.y,
            viewProj,
            f.x, f.y, f.w, f.h,
            0.0f,
            false
        );
    }
}
