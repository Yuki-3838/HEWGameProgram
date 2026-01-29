#include "TitleScene.h"
#include <vector>

// ボタンの当たり判定
bool TitleScene::IsMouseOverButton(const Button& btn, float mouseX, float mouseY)
{
    float left = btn.x - btn.width / 2.0f;
    float right = btn.x + btn.width / 2.0f;
    float top = btn.y - btn.height / 2.0f;
    float bottom = btn.y + btn.height / 2.0f;
    return (mouseX >= left && mouseX <= right && mouseY >= top && mouseY <= bottom);
}

void TitleScene::Init()
{
    m_pCamera = new Camera(1920,1080);
    m_pBackground = nullptr;
    m_pTitleTex = nullptr;

    // 動画プレイヤーの初期化
    m_pVideoPlayer = new VideoPlayer();
    m_pVideoPlayer->Init(L"asset/movie/Title_Movie.mp4", m_pRenderer->GetDevice());
    m_pVideoPlayer->SetLoop(true); // ループ再生

    // ボタンテクスチャの読み込み（なければblockをフォールバック）
    m_pStartButtonTex = m_pResourceManager->LoadTexture("asset/texture/start_button.png", m_pRenderer->GetDevice());
    if (!m_pStartButtonTex) {
        m_pStartButtonTex = m_pResourceManager->LoadTexture("asset/texture/block.png", m_pRenderer->GetDevice());
    }
    m_pExitButtonTex = m_pResourceManager->LoadTexture("asset/texture/exit_button.png", m_pRenderer->GetDevice());
    if (!m_pExitButtonTex) {
        m_pExitButtonTex = m_pResourceManager->LoadTexture("asset/texture/block.png", m_pRenderer->GetDevice());
    }

    // スタートボタンの初期化（左側）
    m_startButton.x = 760.0f;
    m_startButton.y = 850.0f;
    m_startButton.width = 300.0f;
    m_startButton.height = 80.0f;
    m_startButton.isHovered = false;

    // 終了ボタンの初期化（右側、スタートボタンの横）
    m_exitButton.x = 1160.0f;
    m_exitButton.y = 850.0f;
    m_exitButton.width = 300.0f;
    m_exitButton.height = 80.0f;
    m_exitButton.isHovered = false;

    m_IsFinished = false;
    m_IsExitRequested = false;
}

void TitleScene::Update()
{
    // 動画の更新（1/60秒 = 約0.0167秒）
    if (m_pVideoPlayer)
    {
        m_pVideoPlayer->Update(1.0f / 60.0f, m_pRenderer->GetContext());
    }

    // マウス座標を取得
    DirectX::XMFLOAT2 mousePos = m_pInput->GetMousePosition();

    // ボタンのホバー状態を更新
    m_startButton.isHovered = IsMouseOverButton(m_startButton, mousePos.x, mousePos.y);
    m_exitButton.isHovered = IsMouseOverButton(m_exitButton, mousePos.x, mousePos.y);

    // 左クリックでボタン処理
    if (m_pInput->GetMouseLeftTrigger())
    {
        if (m_startButton.isHovered)
        {
            // スタートボタンがクリックされた
            m_IsFinished = true;
        }
        else if (m_exitButton.isHovered)
        {
            // 終了ボタンがクリックされた
            m_IsExitRequested = true;
        }
    }
}

void TitleScene::Draw()
{
    // 背景色でクリア
    float clearColor[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
    m_pRenderer->StartFrame(clearColor);

    DirectX::XMMATRIX viewProj = m_pCamera->GetViewProjection();

    // 動画を背景として描画
    if (m_pVideoPlayer && m_pVideoPlayer->GetSRV())
    {
        // 画面全体に動画を描画
        m_pSpriteRenderer->Draw(m_pRenderer->GetContext(), m_pVideoPlayer->GetSRV(),
            0.0f, 0.0f, 1920.0f, 1080.0f, viewProj);
    }

    // スタートボタン描画
    if (m_pStartButtonTex)
    {
        float scale = m_startButton.isHovered ? 1.1f : 1.0f; // ホバー時に少し大きく
        float w = m_startButton.width * scale;
        float h = m_startButton.height * scale;
        float x = m_startButton.x - w / 2.0f;
        float y = m_startButton.y - h / 2.0f;
        m_pSpriteRenderer->Draw(m_pRenderer->GetContext(), m_pStartButtonTex,
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

void TitleScene::Uninit()
{
    if (m_pVideoPlayer) { delete m_pVideoPlayer; m_pVideoPlayer = nullptr; }
    if (m_pCamera) { delete m_pCamera; m_pCamera = nullptr; }
    if (m_pBackground) { delete m_pBackground; m_pBackground = nullptr; }
}
