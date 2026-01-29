#include "TitleScene.h"
#include <vector>
#include <Windows.h>

void TitleScene::Init()
{
    m_pCamera = new Camera(1280, 720);
    m_pBackground = nullptr;
    m_pTitleTex = nullptr;

    // タイトル動画の初期化
    m_pVideo = new VideoPlayer();
    m_pVideo->Init(L"asset/movie/Title_Movie.mp4", m_pRenderer->GetDevice());
    m_pVideo->SetLoop(true);

    // ボタンサイズ
    float buttonWidth = 200.0f;
    float buttonHeight = 70.0f;
    float buttonY = 510.0f;
    float spacing = 50.0f;  // ボタン間の隙間

    // スタートボタンの初期化（左側）
    m_pStartButtonTex = m_pResourceManager->LoadTexture("asset/texture/object/block.png", m_pRenderer->GetDevice());
    m_StartButtonWidth = buttonWidth;
    m_StartButtonHeight = buttonHeight;
    m_StartButtonX = (1280.0f / 2.0f) - buttonWidth - (spacing / 2.0f);  // 中央より左
    m_StartButtonY = buttonY;

    // 終了ボタンの初期化（右側）
    m_pExitButtonTex = m_pResourceManager->LoadTexture("asset/texture/object/block.png", m_pRenderer->GetDevice());
    m_ExitButtonWidth = buttonWidth;
    m_ExitButtonHeight = buttonHeight;
    m_ExitButtonX = (1280.0f / 2.0f) + (spacing / 2.0f);  // 中央より右
    m_ExitButtonY = buttonY;

    m_IsFinished = false;
}

void TitleScene::Update()
{
    // 動画の更新
    if (m_pVideo)
    {
        m_pVideo->Update(1.0f / 60.0f, m_pRenderer->GetContext());
    }

    // マウス左クリック
    if (m_pInput->GetKeyTrigger(VK_LBUTTON))
    {
        // スタートボタンが押されたらゲーム開始
        if (IsMouseOverButton(m_StartButtonX, m_StartButtonY, m_StartButtonWidth, m_StartButtonHeight))
        {
            m_IsFinished = true;
        }
        // 終了ボタンが押されたらアプリ終了
        if (IsMouseOverButton(m_ExitButtonX, m_ExitButtonY, m_ExitButtonWidth, m_ExitButtonHeight))
        {
            PostMessage(GetForegroundWindow(), WM_CLOSE, 0, 0);
        }
    }

    // Enterキーでもゲーム開始
    if (m_pInput->GetKeyTrigger(VK_RETURN))
    {
        m_IsFinished = true;
    }
}

void TitleScene::Draw()
{
    // 背景を黒でクリア
    float clearColor[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
    m_pRenderer->StartFrame(clearColor);

    DirectX::XMMATRIX viewProj = m_pCamera->GetViewProjection();

    // 動画を画面に描画
    if (m_pVideo && m_pVideo->GetSRV())
    {
        m_pSpriteRenderer->Draw(
            m_pRenderer->GetContext(),
            m_pVideo->GetSRV(),
            0.0f, 0.0f, 1280.0f, 720.0f,
            viewProj
        );
    }

    // スタートボタンを描画（ホバー時は1.15倍に拡大）
    if (m_pStartButtonTex)
    {
        bool hover = IsMouseOverButton(m_StartButtonX, m_StartButtonY, m_StartButtonWidth, m_StartButtonHeight);
        float scale = hover ? 1.15f : 1.0f;
        float drawWidth = m_StartButtonWidth * scale;
        float drawHeight = m_StartButtonHeight * scale;
        float drawX = m_StartButtonX - (drawWidth - m_StartButtonWidth) / 2.0f;
        float drawY = m_StartButtonY - (drawHeight - m_StartButtonHeight) / 2.0f;

        m_pSpriteRenderer->Draw(
            m_pRenderer->GetContext(),
            m_pStartButtonTex,
            drawX, drawY, drawWidth, drawHeight,
            viewProj
        );
    }

    // 終了ボタンを描画（ホバー時は1.15倍に拡大）
    if (m_pExitButtonTex)
    {
        bool hover = IsMouseOverButton(m_ExitButtonX, m_ExitButtonY, m_ExitButtonWidth, m_ExitButtonHeight);
        float scale = hover ? 1.15f : 1.0f;
        float drawWidth = m_ExitButtonWidth * scale;
        float drawHeight = m_ExitButtonHeight * scale;
        float drawX = m_ExitButtonX - (drawWidth - m_ExitButtonWidth) / 2.0f;
        float drawY = m_ExitButtonY - (drawHeight - m_ExitButtonHeight) / 2.0f;

        m_pSpriteRenderer->Draw(
            m_pRenderer->GetContext(),
            m_pExitButtonTex,
            drawX, drawY, drawWidth, drawHeight,
            viewProj
        );
    }

    m_pRenderer->EndFrame();
}

void TitleScene::Uninit()
{
    if (m_pCamera) { delete m_pCamera; m_pCamera = nullptr; }
    if (m_pBackground) { delete m_pBackground; m_pBackground = nullptr; }
    if (m_pVideo) { delete m_pVideo; m_pVideo = nullptr; }
}

bool TitleScene::IsMouseOverButton(float x, float y, float width, float height)
{
    // マウス位置を取得
    POINT pt;
    GetCursorPos(&pt);

    // ウィンドウのクライアント座標に変換
    HWND hWnd = GetForegroundWindow();
    ScreenToClient(hWnd, &pt);

    // ウィンドウサイズを取得
    RECT rc;
    GetClientRect(hWnd, &rc);
    float windowWidth = (float)(rc.right - rc.left);
    float windowHeight = (float)(rc.bottom - rc.top);

    // マウス座標をゲーム座標(1280x720)に変換
    float mouseX = (pt.x / windowWidth) * 1280.0f;
    float mouseY = (pt.y / windowHeight) * 720.0f;

    // ボタン範囲内かチェック
    return (mouseX >= x && mouseX <= x + width &&
            mouseY >= y && mouseY <= y + height);
}
