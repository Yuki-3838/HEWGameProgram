#include "Stage1Scene.h"
#include "iostream"
#include "Collision.h"

void Stage1Scene::Init()
{
    // タイルの情報
    Kaneda::s_TileInfo tileTable[] =
    {
        {false,false,false},    // 空
        {true,false,false},     // 壁
        {false,false,true}      // ゴール
    };
    // リスト作成
    CreateList(maxChara);

    // 1. 各種マネージャー・マップの生成
    m_pTileMap = new TileMap();
    m_pTileMap->LoadCSV("asset/map/Stage1.csv");
    m_pMapRenderer = new MapRenderer();
    m_pCamera = new Camera(3840,2160);

    // 2. プレイヤーの生成と初期化
    m_pCharaList[0] = AddList(State::CharaType::t_Player);
    m_pCharaList[1] = AddList(State::CharaType::t_Enemy);

    // 3. テクスチャのロード
    m_pMapTex = m_pResourceManager->LoadTexture("asset/texture/card.jpg", m_pRenderer->GetDevice());
    m_pPlayerTexIdle = m_pResourceManager->LoadTexture("asset/texture/T_Stand_B.png", m_pRenderer->GetDevice());
    m_pPlayerTexWalk = m_pResourceManager->LoadTexture("asset/texture/T_Dash_A.png", m_pRenderer->GetDevice());
    m_pPlayerTexJump = m_pResourceManager->LoadTexture("asset/texture/testSP.png", m_pRenderer->GetDevice());
    m_pEnemyTex = m_pResourceManager->LoadTexture("asset/texture/nazuna.jpg", m_pRenderer->GetDevice());
    

    // プレイヤーにテクスチャを渡す
    Player* player = dynamic_cast<Player*>(m_pCharaList[0]);
    if (player)
    {
        // ★ここで3枚セットで渡す
        player->SetTextures(m_pPlayerTexIdle, m_pPlayerTexWalk,m_pPlayerTexJump);

        // 最初の初期化 (Init) も呼んでおく
        player->Init(m_pPlayerTexIdle); //Idleを渡す
    }
    m_pCharaList[1]->Init(m_pEnemyTex);
    m_IsFinished = false;
}

void Stage1Scene::Update()
{
    // 現在のキャラクターの数だけ更新
    for (int i = 0; i < m_currentCharaNum; i++)
    {
        if (m_pCharaList[i])
        {
            m_pCharaList[i]->Update(*m_pTileMap);
        }
    }
    // シーン終了判定
    if (m_pInput->GetKeyTrigger(VK_RETURN))
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
    m_pCamera->SetPosition(m_pCharaList[static_cast<int>(State::CharaType::t_Player)]->GetPosition().x - 240, m_pCharaList[static_cast<int>(State::CharaType::t_Player)]->GetPosition().y - 540);
    DirectX::XMMATRIX viewProj = m_pCamera->GetViewProjection();

    //1. マップの描画
    m_pMapRenderer->Draw(m_pRenderer->GetContext(), m_pSpriteRenderer, *m_pTileMap, m_pMapTex, viewProj);

    // 2. プレイヤーの描画
    for (int i = 0; i < m_currentCharaNum; i++)
    {
        if (m_pCharaList[i])
        {
             m_pCharaList[i]->Draw(m_pRenderer->GetContext(), m_pSpriteRenderer, viewProj);
        }
    }
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
    AllClearList(m_pCharaList);
}

void Stage1Scene::CreateList(int num)
{
    if (m_pCharaList == nullptr)
    {
        m_pCharaList = new Character * [num];
        
    }
}

void Stage1Scene::ClearList(Character* list)
{
    delete list;
    list = nullptr;
    m_currentCharaNum--;
}

void Stage1Scene::AllClearList(Character** list)
{
    for (int i = 0; i < m_currentCharaNum; i++)
    {
        if (list[i] != nullptr)
        {
            delete list[i];
            list[i] = nullptr;
        }
    }
}

Character* Stage1Scene::AddList(State::CharaType e_name)
{
    m_currentCharaNum++;
    switch (e_name)
    {
    case State::CharaType::t_Player:
        return new Player;
        break;
    case State::CharaType::t_Enemy:
        return new Enemy;
        break;
    };
    
}

void Stage1Scene::TileCollision(int charaName)
{
    float left = m_pCharaList[charaName]->GetPosition().x;
    float right = (m_pCharaList[charaName]->GetPosition().x + m_pCharaList[charaName]->GetSize().x);
    float top = m_pCharaList[charaName]->GetPosition().y;
    float bottom = (m_pCharaList[charaName]->GetPosition().y + m_pCharaList[charaName]->GetSize().y);

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
                
            }
        }
    }
}