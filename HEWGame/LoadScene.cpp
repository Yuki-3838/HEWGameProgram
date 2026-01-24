#include "LoadScene.h"
#include "Stage1Scene.h" // Stage1で使うアセットを知るため

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
    m_pLoadTex = m_pResourceManager->LoadTexture("asset/texture/kinnniku.png", m_pRenderer->GetDevice());

    //スレッド作成：裏で BackgroundLoad 関数を実行させる！
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

        m_pResourceManager->LoadTexture("asset/texture/card.jpg", device);
        m_pResourceManager->LoadTexture("asset/texture/T_Stand_B.png", device);
        m_pResourceManager->LoadTexture("asset/texture/Anime_Hero_Dash.png", device);
        m_pResourceManager->LoadTexture("asset/texture/testSP.png", device);
        m_pResourceManager->LoadTexture("asset/texture/nazuna.jpg", device);
        m_pResourceManager->LoadTexture("asset/texture/Test_dash_Effect.png", device);

        // ※必要ならSoundなどもここで先読みできますが、まずは画像でOK
    }

    // 読み込みが終わったらフラグを立てる
    //ちょっと見せるよう
    std::this_thread::sleep_for(std::chrono::milliseconds(1000)); 

    m_IsFinished = true; // 完了！
}

void LoadScene::Update()
{
    // メインスレッドはここでアニメーションの計算をするだけ
    // もし m_IsFinished が true になったら、Game.cpp が感知してシーンを切り替えてくれます
}

void LoadScene::Draw()
{
    float clearColor[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
    m_pRenderer->StartFrame(clearColor);

    DirectX::XMMATRIX viewProj = m_pCamera->GetViewProjection();
    if (m_pLoadTex)
    {
        // ローディングアイコンをくるくる回す演出
        static float angle = 0.0f;
        angle += 5.0f; // 回転させる

        // 画面中央に描画
        m_pSpriteRenderer->Draw(
            m_pRenderer->GetContext(),
            m_pLoadTex,
            0.0f, 0.0f,      // 座標
            1280.0f, 960.0f,      // サイズ
            viewProj,
            0.0f, 0.0f, 128.0f, 128.0f, // UV
            angle
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
