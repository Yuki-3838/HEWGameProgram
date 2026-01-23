#include "ResultScene.h"
#include <vector>

void ResultScene::Init() 
{
    // リザルト画面用の画像を読み込み
    m_pResultTex = m_pResourceManager->LoadTexture("assets/texture/result.png", m_pRenderer->GetDevice());

    // 遷移フラグをリセット
    m_IsFinished = false;
}

void ResultScene::Update() 
{
    // スペースキーまたはエンターキーが押されたら、再びタイトルへ戻る
    if (m_pInput->GetKeyTrigger(VK_SPACE)) 
    {
        m_IsFinished = true;
    }
}

void ResultScene::Draw() 
{
    float clearColor[4] = { 0.f, 0.f, 1.0f, 1.0f };
    m_pRenderer->StartFrame(clearColor);

    // リザルト画像を表示
    if (m_pResultTex) 
    {
        // 画面中央に表示（座標やサイズは適宜調整）
        m_pSpriteRenderer->Draw(
            m_pRenderer->GetContext(),
            m_pResultTex,
            0.0f, 0.0f, 600.0f, 300.0f,
            DirectX::XMMatrixIdentity() // リザルトもスクロール不要なのでIdentity
        );
    }

    m_pRenderer->EndFrame();
}

void ResultScene::Uninit()
{
    if(m_pResultTex)
    {
		m_pResultTex->Release();
	}
    m_pResultTex = nullptr;
}