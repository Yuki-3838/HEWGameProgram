#include "ResultScene.h"
#include <vector>
#include <Windows.h>

void ResultScene::Init()
{
    m_pCamera = new Camera(1280, 720);

    // リザルト画面用の画像を読み込み
    m_pResultTex = m_pResourceManager->LoadTexture("assets/texture/result.png", m_pRenderer->GetDevice());

    // ボタンサイズ
    float buttonWidth = 200.0f;
    float buttonHeight = 70.0f;
    float buttonY = 510.0f;
    float spacing = 50.0f;  // ボタン間の隙間

    // タイトルに戻るボタンの初期化（左側）
    m_pTitleButtonTex = m_pResourceManager->LoadTexture("asset/texture/object/block.png", m_pRenderer->GetDevice());
    m_TitleButtonWidth = buttonWidth;
    m_TitleButtonHeight = buttonHeight;
    m_TitleButtonX = (1280.0f / 2.0f) - buttonWidth - (spacing / 2.0f);
    m_TitleButtonY = buttonY;

    // 終了ボタンの初期化（右側）
    m_pExitButtonTex = m_pResourceManager->LoadTexture("asset/texture/object/block.png", m_pRenderer->GetDevice());
    m_ExitButtonWidth = buttonWidth;
    m_ExitButtonHeight = buttonHeight;
    m_ExitButtonX = (1280.0f / 2.0f) + (spacing / 2.0f);
    m_ExitButtonY = buttonY;

    m_IsFinished = false;
}

void ResultScene::Update()
{
    // マウス左クリック
    if (m_pInput->GetKeyTrigger(VK_LBUTTON))
    {
        // タイトルボタンが押されたらタイトルへ戻る
        if (IsMouseOverButton(m_TitleButtonX, m_TitleButtonY, m_TitleButtonWidth, m_TitleButtonHeight))
        {
            m_IsFinished = true;
        }
        // 終了ボタンが押されたらアプリ終了
        if (IsMouseOverButton(m_ExitButtonX, m_ExitButtonY, m_ExitButtonWidth, m_ExitButtonHeight))
        {
            PostMessage(GetForegroundWindow(), WM_CLOSE, 0, 0);
        }
    }

    // スペースキーでもタイトルへ戻る
    if (m_pInput->GetKeyTrigger(VK_SPACE))
    {
        m_IsFinished = true;
    }
}

void ResultScene::Draw()
{
    float clearColor[4] = { 0.f, 0.f, 0.2f, 1.0f };
    m_pRenderer->StartFrame(clearColor);

    DirectX::XMMATRIX viewProj = m_pCamera->GetViewProjection();

    // リザルト画像を表示
    if (m_pResultTex)
    {
        m_pSpriteRenderer->Draw(
            m_pRenderer->GetContext(),
            m_pResultTex,
            0.0f, 0.0f, 1280.0f, 720.0f,
            viewProj
        );
    }

    // タイトルボタンを描画（ホバー時は1.15倍に拡大）
    if (m_pTitleButtonTex)
    {
        bool hover = IsMouseOverButton(m_TitleButtonX, m_TitleButtonY, m_TitleButtonWidth, m_TitleButtonHeight);
        float scale = hover ? 1.15f : 1.0f;
        float drawWidth = m_TitleButtonWidth * scale;
        float drawHeight = m_TitleButtonHeight * scale;
        float drawX = m_TitleButtonX - (drawWidth - m_TitleButtonWidth) / 2.0f;
        float drawY = m_TitleButtonY - (drawHeight - m_TitleButtonHeight) / 2.0f;

        m_pSpriteRenderer->Draw(
            m_pRenderer->GetContext(),
            m_pTitleButtonTex,
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

void ResultScene::Uninit()
{
    if (m_pCamera) { delete m_pCamera; m_pCamera = nullptr; }
    if (m_pResultTex)
    {
        m_pResultTex->Release();
        m_pResultTex = nullptr;
    }
}

bool ResultScene::IsMouseOverButton(float x, float y, float width, float height)
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
