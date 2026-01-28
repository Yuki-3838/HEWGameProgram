#include "DemoReelScene.h"
//pushするためのコメント
void DemoReelScene::Init()
{
	m_pCamera = new Camera(1280, 720);
	m_pVideo = new VideoPlayer();
	m_pVideo->Init(L"asset/movie/op_1.mp4", m_pRenderer->GetDevice());
	m_pVideo->SetLoop(true); // ループ再生設定
	m_IsFinished = false;
}

void DemoReelScene::Update()
{
	m_pVideo->Update(1.0f/60.0f, m_pRenderer->GetContext());
	// スペースキーが押されたら次のシーン（Stage1）へ
	if (m_pInput->GetKeyTrigger(VK_SPACE))
	{
		m_IsFinished = true;
	}
}

void DemoReelScene::Draw()
{
	float clearColor[4] = { 0.0f, 0.0f, 0.0f, 1.0f }; // 黒色
	m_pRenderer->StartFrame(clearColor);

	// --- 動画の描画 ---
	Camera movieCamera(1280, 720);
	m_pSpriteRenderer->Draw(
		m_pRenderer->GetContext(),
		m_pVideo->GetSRV(),
		0.0f, 0.0f, 1280.0f, 720.0f,
		movieCamera.GetViewProjection()
	);

	m_pRenderer->EndFrame();
}

void DemoReelScene::Uninit()
{
	if (m_pCamera) { delete m_pCamera; m_pCamera = nullptr; }

}
