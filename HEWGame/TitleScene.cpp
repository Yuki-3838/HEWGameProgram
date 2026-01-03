#include "TitleScene.h"
#include <vector>

void TitleScene::Init() 
{
    m_pCamera = new Camera(1280, 720);
    // タイトル画像の読み込み
    m_pBackground = new BackGround(); // メンバ変数として保持
    m_pTitleTex = m_pResourceManager->LoadTexture("assets/texture/kinnniku.png", m_pRenderer->GetDevice());

    if (m_pTitleTex) 
    {
        m_pBackground->Init(m_pTitleTex);
        m_pBackground->SetSize(1280.0f, 720.0f); // 画面サイズに合わせる
        m_pBackground->SetPosition(0.0f, 0.0f);
    }
    m_IsFinished = false;
}

void TitleScene::Update() 
{
    // スペースキーが押されたら次のシーン（Stage1）へ
    if (m_pInput->GetKeyTrigger(VK_SPACE))
    {
        m_IsFinished = true;
    }
}

void TitleScene::Draw()
{
    // 背景を黒でクリア
    float clearColor[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
    m_pRenderer->StartFrame(clearColor);

    // タイトル画像を画面中央付近に表示
    DirectX::XMMATRIX viewProj = m_pCamera->GetViewProjection();
    if (m_pBackground) 
    {
        // 背景オブジェクトに描画を任せる
        m_pBackground->Draw(m_pRenderer->GetContext(), m_pSpriteRenderer, viewProj);
    }

    m_pRenderer->EndFrame();
}

void TitleScene::Uninit()
{
    if (m_pCamera) { delete m_pCamera; m_pCamera = nullptr; }
    if (m_pBackground) { delete m_pBackground; m_pBackground = nullptr; }
}