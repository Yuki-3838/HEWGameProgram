#include "ResultScene.h"
#include <vector>
#include "GameData.h"

void ResultScene::Init() 
{
    // リザルト画面用の画像を読み込み
    m_pResultTex = m_pResourceManager->LoadTexture("assets/texture/result.png", m_pRenderer->GetDevice());

	m_pGameUI = new GameUI();
	m_pGameUI->Init(m_pRenderer->GetDevice(), m_pResourceManager);
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

	int score = GameData::GetScore();
	int time = static_cast<int>(GameData::GetTime());
    if (m_pGameUI)
    {
		//タイム表示
		m_pGameUI->DrawNumber(m_pRenderer->GetContext(), m_pSpriteRenderer,time,800.0f,400.0f,1.0f);

		//スコア表示
        m_pGameUI->DrawNumber(m_pRenderer->GetContext(), m_pSpriteRenderer, score, 800.0f, 600.0f, 1.0f);
    }

    m_pRenderer->EndFrame();
}

void ResultScene::Uninit()
{
    if(m_pResultTex)
    {
		m_pResultTex->Release();
        m_pResultTex = nullptr;
	}
    if (m_pGameUI)
    {
        m_pGameUI->Uninit();
        delete m_pGameUI;
		m_pGameUI = nullptr;
    }
}
