#include "Stage1Scene.h"
#include "Collision.h"
#include "iostream"
void Stage1Scene::Init()
{

    // === 金田作成 ===//
    // タイルの情報
    Kaneda::s_TileInfo tileTable[] =
    {
        {false,false,false},    // 空
        {true,false,false},     // 壁
        {false,false,true}      // ゴール
    };

    //============================//


    // 1. 各種マネージャー・マップの生成
    m_pTileMap = new TileMap();
    m_pTileMap->LoadCSV("asset/map/Stage1.csv");
    m_pMapRenderer = new MapRenderer();
    m_pCamera = new Camera(1920,1080);


    // 2. プレイヤーの生成と初期化
    m_pPlayer = new Player();

    // 3. テクスチャのロード
    m_pMapTex = m_pResourceManager->LoadTexture("asset/texture/kinnniku.png", m_pRenderer->GetDevice());
    m_pPlayerTex = m_pResourceManager->LoadTexture("asset/texture/kinnniku.png", m_pRenderer->GetDevice());

    // プレイヤーにテクスチャを渡す
    m_pPlayer->Init(m_pPlayerTex);

    m_IsFinished = false;
    m_pPlayer->SetPosition(0.0f, 0.0f); // 画面内に強制配置
    m_pPlayer->SetSize(200.0f, 200.0f);     // 大きめに表示
}

void Stage1Scene::Update()
{
    // プレイヤーの更新（キー入力移動）
    if (m_pPlayer)
    {
        m_pPlayer->Update();
    }

    // プレイヤーからステージへの当たり判定（仮）
    if (m_pPlayer)
    {
        // 画面サイズ / マップサイズ　＝　1マスのサイズ
        // 画面サイズ / １マスのサイズ　＝　マスの量
        // プレイヤー座標＋表示サイズ/2　と　近くのマップの座標を計算する
        float sizeX = m_pMapRenderer->GetSizex() / m_pTileMap->GetWidth();  // 横の１マス当たりの大きさ
        float sizeY = m_pMapRenderer->GetSizey() / m_pTileMap->GetHeight(); // 縦の１マス当たりの大きさ


        float left = m_pPlayer->GetPosition().x;
        float right = (m_pPlayer->GetPosition().x + m_pPlayer->GetSize().x);
        float top = m_pPlayer->GetPosition().y;
        float bottom = (m_pPlayer->GetPosition().y + m_pPlayer->GetSize().y);

        int tileX_L = static_cast<int>(left / 32);
        int tileX_R = static_cast<int>(right / 32);
        int tileY_T = static_cast<int>(top / 32);
        int tileY_B = static_cast<int>(bottom / 32);

        for (int y = top; y <= bottom; y++)
        {
            for (int x = left; x <= right; x++)
            {
                if (m_pTileMap->GetTileID(x, y) == Kaneda::TILE_WALL)
                {
                    int a;
                }
                else if (m_pTileMap->GetTileID(x, y) == Kaneda::TILE_EMPTY)
                {
                    int a;
                }
            }
        }
    }

    // シーン終了判定
    if (m_pInput->GetKeyTrigger(VK_SPACE))
    {
        m_IsFinished = true;
    }
}

void Stage1Scene::Draw()
{
    // 背景色クリア（空の色）
    float clearColor[4] = { 0.f, 1.f, 0.f, 1.0f };
    m_pRenderer->StartFrame(clearColor);

    // カメラ行列の取得
    DirectX::XMMATRIX viewProj = m_pCamera->GetViewProjection();

    //1. マップの描画
    m_pMapRenderer->Draw(m_pRenderer->GetContext(), m_pSpriteRenderer, *m_pTileMap, m_pMapTex, viewProj);

    // 2. プレイヤーの描画
    if (m_pPlayer)
    {
        m_pPlayer->Draw(m_pRenderer->GetContext(), m_pSpriteRenderer, viewProj);
    }

    m_pRenderer->EndFrame();
}

void Stage1Scene::Uninit()
{
    // メモリ解放
    if (m_pPlayer) { delete m_pPlayer; m_pPlayer = nullptr; }
    if (m_pTileMap) { delete m_pTileMap; m_pTileMap = nullptr; }
    if (m_pMapRenderer) { delete m_pMapRenderer; m_pMapRenderer = nullptr; }
    if (m_pCamera) { delete m_pCamera; m_pCamera = nullptr; }
}