#include "ResultScene.h"
#include <vector>

// ボタンの当たり判定
bool ResultScene::IsMouseOverButton(const ResultButton& btn, float mouseX, float mouseY)
{
    float left = btn.x - btn.width / 2.0f;
    float right = btn.x + btn.width / 2.0f;
    float top = btn.y - btn.height / 2.0f;
    float bottom = btn.y + btn.height / 2.0f;
    return (mouseX >= left && mouseX <= right && mouseY >= top && mouseY <= bottom);
}

void ResultScene::Init()
{
    m_pCamera = new Camera(1920, 1080);

    // リザルト画面用の画像を読み込む
    m_pResultTex = m_pResourceManager->LoadTexture("asset/texture/result.png", m_pRenderer->GetDevice());

    // ボタンテクスチャの読み込み（なければblockをフォールバック）
    m_pTitleButtonTex = m_pResourceManager->LoadTexture("asset/texture/title_button.png", m_pRenderer->GetDevice());
    if (!m_pTitleButtonTex) {
        m_pTitleButtonTex = m_pResourceManager->LoadTexture("asset/texture/block.png", m_pRenderer->GetDevice());
    }
    m_pExitButtonTex = m_pResourceManager->LoadTexture("asset/texture/exit_button.png", m_pRenderer->GetDevice());
    if (!m_pExitButtonTex) {
        m_pExitButtonTex = m_pResourceManager->LoadTexture("asset/texture/block.png", m_pRenderer->GetDevice());
    }

    // タイトルに戻るボタンの初期化（画面中央やや下）
    m_titleButton.x = 960.0f;
    m_titleButton.y = 600.0f;
    m_titleButton.width = 300.0f;
    m_titleButton.height = 80.0f;
    m_titleButton.isHovered = false;

    // 終了ボタンの初期化（タイトルボタンの下）
    m_exitButton.x = 960.0f;
    m_exitButton.y = 720.0f;
    m_exitButton.width = 300.0f;
    m_exitButton.height = 80.0f;
    m_exitButton.isHovered = false;

    // 遷移フラグをリセット
    m_IsFinished = false;
    m_IsExitRequested = false;
}

void ResultScene::Update()
{
    // マウス座標を取得
    DirectX::XMFLOAT2 mousePos = m_pInput->GetMousePosition();

    // ボタンのホバー状態を更新
    m_titleButton.isHovered = IsMouseOverButton(m_titleButton, mousePos.x, mousePos.y);
    m_exitButton.isHovered = IsMouseOverButton(m_exitButton, mousePos.x, mousePos.y);

    // 左クリックでボタン処理
    if (m_pInput->GetMouseLeftTrigger())
    {
        if (m_titleButton.isHovered)
        {
            // タイトルに戻るボタンがクリックされた
            m_IsFinished = true;
        }
        else if (m_exitButton.isHovered)
        {
            // 終了ボタンがクリックされた
            m_IsExitRequested = true;
        }
    }
}

void ResultScene::Draw()
{
    float clearColor[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
    m_pRenderer->StartFrame(clearColor);

    DirectX::XMMATRIX viewProj = m_pCamera->GetViewProjection();

    // リザルト画像を表示
    if (m_pResultTex)
    {
        m_pSpriteRenderer->Draw(
            m_pRenderer->GetContext(),
            m_pResultTex,
            0.0f, 0.0f, 1920.0f, 1080.0f,
            viewProj
        );
    }

    // タイトルに戻るボタン描画
    if (m_pTitleButtonTex)
    {
        float scale = m_titleButton.isHovered ? 1.1f : 1.0f;
        float w = m_titleButton.width * scale;
        float h = m_titleButton.height * scale;
        float x = m_titleButton.x - w / 2.0f;
        float y = m_titleButton.y - h / 2.0f;
        m_pSpriteRenderer->Draw(m_pRenderer->GetContext(), m_pTitleButtonTex,
            x, y, w, h, viewProj);
    }

    // 終了ボタン描画
    if (m_pExitButtonTex)
    {
        float scale = m_exitButton.isHovered ? 1.1f : 1.0f;
        float w = m_exitButton.width * scale;
        float h = m_exitButton.height * scale;
        float x = m_exitButton.x - w / 2.0f;
        float y = m_exitButton.y - h / 2.0f;
        m_pSpriteRenderer->Draw(m_pRenderer->GetContext(), m_pExitButtonTex,
            x, y, w, h, viewProj);
    }

    m_pRenderer->EndFrame();
}

void ResultScene::Uninit()
{
    if (m_pCamera) { delete m_pCamera; m_pCamera = nullptr; }
}
