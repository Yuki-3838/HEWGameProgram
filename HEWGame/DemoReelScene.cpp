#include "DemoReelScene.h"
//pushするためのコメント
void DemoReelScene::Init()
{
	m_pCamera = new Camera(1280, 720);
	m_pVideo = new VideoPlayer();
	m_pVideo->Init(L"asset/movie/op.mp4", m_pRenderer->GetDevice());
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
	Camera movieCamera(1280, 720);

	m_pSpriteRenderer->Draw(
		m_pRenderer->GetContext(),
		m_pVideo->GetSRV(),
		0.0f, 0.0f, 1280.0f, 720.0f,
		movieCamera.GetViewProjection()
	);
}

void DemoReelScene::Uninit()
{
	if (m_pCamera) { delete m_pCamera; m_pCamera = nullptr; }
	if (m_pVideo) { delete m_pVideo; m_pVideo = nullptr; }
}
