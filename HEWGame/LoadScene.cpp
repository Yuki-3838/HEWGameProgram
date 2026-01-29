#include "LoadScene.h"
#include "Stage1Scene.h" // Stage1で使うアセットを知るため

static constexpr float SCREEN_W  = 1920.0f;
static constexpr float SCREEN_H  = 1080.0f;

LoadScene::LoadScene(Renderer* r, ResourceManager* res, SpriteRenderer* spr, Input* input, NextSceneType next)
    : Scene(r, res, spr, input), m_NextScene(next)
{
    m_pThread = nullptr;
    m_IsFinished = false; // 最初は終わってない
    m_pCamera = nullptr;
}

void LoadScene::Init()
{
    m_pCamera = new Camera(1920, 1080);
    m_pCamera->SetPosition(0.0f, 0.0f);
    // ロード画面自体の画像（これは即座に必要なので普通に読む）
    m_pLoadMainTex = m_pResourceManager->LoadTexture("asset/texture/Load/Road_Main.png", m_pRenderer->GetDevice());
    m_pLoadRedBarTex = m_pResourceManager->LoadTexture("asset/texture/Load/Road_Bg_red.png", m_pRenderer->GetDevice());

	//ロード画面のくるくる回るアイコン
    m_pLoadCycle1Tex = m_pResourceManager->LoadTexture("asset/texture/Load/Road_Cycle_1.png", m_pRenderer->GetDevice());
    m_pLoadCycle2Tex = m_pResourceManager->LoadTexture("asset/texture/Load/Road_Cycle_2.png", m_pRenderer->GetDevice());
    m_pLoadCycle3Tex = m_pResourceManager->LoadTexture("asset/texture/Load/Road_Cycle_3.png", m_pRenderer->GetDevice());

    //アニメーション初期値
	m_RedBarX = -500.0f;
	m_Angle = 0.0f;
    //スレッド作成：裏で BackgroundLoad 関数を実行させる
    //this を渡しているのは、メンバ関数を実行するため
    m_pThread = new std::thread(&LoadScene::BackgroundLoad, this);
}

//この関数が「裏側（別スレッド）」で動きます
void LoadScene::BackgroundLoad()
{
    // ID3D11Device* はスレッドセーフ（同時アクセスOK）なので、
    // 裏スレッドからResourceManagerを呼んでも大丈夫です。
    // ※ただし、ResourceManager::LoadTextureが内部で書き換える m_Resources (map) は
    // メインスレッドから同時に書き換えると危険ですが、
    // 今LoadSceneはDraw(読み取り)しかしていないので大丈夫です。

    ID3D11Device* device = m_pRenderer->GetDevice();

    if (m_NextScene == NextSceneType::Stage1)
    {
        // --- Stage1で使う重たい画像を全部ここで読んでおく ---
        // ResourceManagerは「すでに読んだものはキャッシュする」ので、
        // ここで読んでおけば、Stage1Scene::Init() の時は一瞬で終わります。

        //m_pResourceManager->LoadTexture("asset/texture/card.jpg", device);
        //m_pResourceManager->LoadTexture("asset/texture/T_Stand_B.png", device);
        //m_pResourceManager->LoadTexture("asset/texture/Anime_Hero_Dash.png", device);
        //m_pResourceManager->LoadTexture("asset/texture/testSP.png", device);
        //m_pResourceManager->LoadTexture("asset/texture/nazuna.jpg", device);
        //m_pResourceManager->LoadTexture("asset/texture/Test_dash_Effect.png", device);
        //=== テクスチャの読込 ===//
        // プレイヤー
        m_pResourceManager->LoadTexture("asset/texture/object/block.png", device);
        m_pResourceManager->LoadTexture("asset/texture/Player/Anime_Hero_Idol.png", device);
        m_pResourceManager->LoadTexture("asset/texture/Player/Anime_Hero_Dash.png", device);
        m_pResourceManager->LoadTexture("asset/texture/Player/Anime_Hero_Jump.png", device);
        m_pResourceManager->LoadTexture("asset/texture/Player/Anime_Hero_Fall.png", device);
        m_pResourceManager->LoadTexture("asset/texture/Player/Anime_Hero_Attack_D.png", device);
        m_pResourceManager->LoadTexture("asset/texture/Player/Anime_Hero_AbilityStay.png", device);
        m_pResourceManager->LoadTexture("asset/texture/Player/Anime_Hero_AbilityDash.png", device);
        m_pResourceManager->LoadTexture("asset/texture/Player/Anime_Hero_AbilityStay.png",device);
        // エネミー
        m_pResourceManager->LoadTexture("asset/texture/Sw_Idole.png", device);
        m_pResourceManager->LoadTexture("asset/texture/Sw_Walk.png", device);
        m_pResourceManager->LoadTexture("asset/texture/Gu_Walk.png", device);

        m_pResourceManager->LoadTexture("asset/texture/Gu_Idole.png", device);
        m_pResourceManager->LoadTexture("asset/texture/Gu_Walk.png", device);
        m_pResourceManager->LoadTexture("asset/texture/Gu_Walk.png", device);

        m_pResourceManager->LoadTexture("asset/texture/Se_Idole.png", device);
        m_pResourceManager->LoadTexture("asset/texture/Se_Walk.png", device);
        m_pResourceManager->LoadTexture("asset/texture/Gu_Walk.png", device);
        // 背景
        m_pResourceManager->LoadTexture("asset/texture/Back/bg_front.png", device); // 手前
        m_pResourceManager->LoadTexture("asset/texture/Back/bg_mid.png", device);   // 中
        m_pResourceManager->LoadTexture("asset/texture/Back/bg_back.png", device);  // 奥

        // ※必要ならSoundなどもここで先読みできまる
    }

    // 読み込みが終わったらフラグを立てる
    //ちょっと見せるよう
    std::this_thread::sleep_for(std::chrono::milliseconds(1000)); 

    m_IsFinished = true; // 完了！
}

void LoadScene::Update()
{
    // メインスレッドはここでアニメーションの計算をするだけ
    // もし m_IsFinished が true になったら、Game.cpp が感知してシーンを切り替えてくれる

	m_RedBarX += 25.0f;

    if (m_RedBarX > SCREEN_W)
    {
		m_RedBarX = -400.0f;
    }

	m_Angle += 3.0f;//回転スピード

	if (m_Angle > 360.0f) m_Angle -= 360.0f;
}

void LoadScene::Draw()
{
    float clearColor[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
    m_pRenderer->StartFrame(clearColor);

    //中心を基準にするためのオフセット
	const float HW = SCREEN_W / 2.0f;
	const float HH = SCREEN_H / 2.0f;
    DirectX::XMMATRIX viewProj = m_pCamera->GetViewProjection();
    if (m_pLoadMainTex)
    {
        // 画面中央に描画
        m_pSpriteRenderer->Draw(
            m_pRenderer->GetContext(),
            m_pLoadMainTex,
            0.0, 0.0f,      // 座標
            SCREEN_W, SCREEN_H,      // サイズ
            viewProj,
			0.0f, 0.0f, 0.0f, 0.0f,
            0.0f,
			false
        );
    }

  //  if (m_pLoadRedBarTex)
  //  {
		//float barLen = 500.0f;
		//float barThick = 500.0f;

  //      float rotAngle = -3.14159f / 2.0f;
		//
  //      float centerX = -HW + m_RedBarX;

  //      float topY = -280.0f;
  //      float bottomY = 280.0f;;

  //      float offX = barThick / 2.0f;
  //      float offY = barLen / 2.0f;
  //      m_pSpriteRenderer->Draw(
  //          m_pRenderer->GetContext(),
  //          m_pLoadRedBarTex,
  //          centerX - offX, topY - offY,
  //          barThick,barLen,      // 座標
  //          viewProj,
  //          0.0f, 0.0f, 
  //          0.0f, 0.0f,
  //          0.0f,
  //          false
  //      );
  //  }

	float cycleSize = 200.0f;
	float drawCycleX = HW + 855.0f;
	float drawCycleY = HH + 370.0f;
    if(m_pLoadCycle1Tex)
    {
        m_pSpriteRenderer->Draw(
            m_pRenderer->GetContext(),
            m_pLoadCycle1Tex,
            drawCycleX - cycleSize * 0.5f, drawCycleY - cycleSize * 0.5f,
            cycleSize, cycleSize,
            viewProj,
            0.0f, 0.0f,
            0.0f, 0.0f,
            m_Angle,
            false
        );
	}

    if (m_pLoadCycle2Tex)
    {
        m_pSpriteRenderer->Draw(
            m_pRenderer->GetContext(),
            m_pLoadCycle2Tex,
            drawCycleX - cycleSize * 0.5f, drawCycleY - cycleSize * 0.5f,
            cycleSize, cycleSize,
            viewProj,
            0.0f, 0.0f,
            0.0f, 0.0f,
            m_Angle * 1.5f,
            false
        );
    }
    if (m_pLoadCycle3Tex)
    {
        m_pSpriteRenderer->Draw(
            m_pRenderer->GetContext(),
            m_pLoadCycle3Tex,
            drawCycleX - cycleSize * 0.5f, drawCycleY - cycleSize * 0.5f,
            cycleSize, cycleSize,
            viewProj,
            0.0f, 0.0f,
            0.0f, 0.0f,
            m_Angle * 0.5,
            false
        );
    }
    m_pRenderer->EndFrame();
}

void LoadScene::Uninit()
{
    //スレッドの後始末
    if (m_pThread)
    {
        //もしスレッドがまだ動いていたら、終わるのを待つ（join）
        if (m_pThread->joinable())
        {
            m_pThread->join();
        }
        delete m_pThread;
        m_pThread = nullptr;
    }
    if (m_pCamera)
    {
        delete m_pCamera;
        m_pCamera = nullptr;
    }
}
