#include "TitleScene.h"
#include <vector>
void TitleScene::Init() 
{
    m_pCamera = new Camera(1920,1080);
    // タイトル画像の読み込み
    m_pBackground = new BackGround(); // メンバ変数として保持
    m_pTitleTex = m_pResourceManager->LoadTexture("asset/texture/testSP.png", m_pRenderer->GetDevice());
    m_pVideo = new VideoPlayer();
    m_pVideo->Init(L"asset/movie/Title_op.mp4", m_pRenderer->GetDevice());
    m_pVideo->SetLoop(true); // ループ再生設定
    if (m_pTitleTex) 
    {
		int animCount = 18;//何分割か
		int xCount = 6;//横に何個並んでいるか
		float frameWidth = 320.0f;
		float frameHeight = 320.0f;
		float speed = 0.2f; // 1コマあたりの表示時間（秒）
        m_pBackground->Init(m_pTitleTex, animCount, xCount, frameWidth, frameHeight, speed);
        m_pBackground->SetSize(1280.0f, 720.0f); // 画面サイズに合わせる
        m_pBackground->SetPosition(0.0f, 0.0f);
    }
    m_IsFinished = false;
}

void TitleScene::Update() 
{
    m_pVideo->Update(1.0f / 60.0f, m_pRenderer->GetContext());
    if (m_pBackground)
    {
        m_pBackground->Update();
    }
    // スペースキーが押されたら次のシーン（Stage1）へ
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

    //// タイトル画像を画面中央付近に表示
    //DirectX::XMMATRIX viewProj = m_pCamera->GetViewProjection();
    //if (m_pBackground) 
    //{
    //    // 背景オブジェクトに描画を任せる
    //    m_pBackground->Draw(m_pRenderer->GetContext(), m_pSpriteRenderer, viewProj);
    //}
        // --- 動画の描画 ---
    Camera movieCamera(1920, 1080);
    m_pSpriteRenderer->Draw(
        m_pRenderer->GetContext(),
        m_pVideo->GetSRV(),
        0.0f, 0.0f, 1920.0f, 1080.0f,
        movieCamera.GetViewProjection()
    );
    m_pRenderer->EndFrame();
}

void TitleScene::Uninit()
{
    if (m_pCamera) { delete m_pCamera; m_pCamera = nullptr; }
    if (m_pBackground) { delete m_pBackground; m_pBackground = nullptr; }
    if (m_pVideo) { delete m_pVideo; m_pVideo = nullptr; }
}
